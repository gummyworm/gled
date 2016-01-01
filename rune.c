#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include "rune.h"
#include "matrix.h"
#include "util.h"
#include "uthash.h"

#define MAX_RUNE_PAGES 512

/* pixelFmt is the pixel format for textures destined for OpenGL */
static SDL_PixelFormat pixelFmt = {
  .palette = 0,
  .BitsPerPixel = 32,
  .BytesPerPixel = 4,
  .Rshift = 0,
  .Rloss = 0,
  .Gloss = 0,
  .Bloss = 0,
  .Aloss = 0,
  .Rmask = 0x000000ff,
  .Gshift = 8,
  .Gmask = 0x0000ff00,
  .Bshift = 16,
  .Bmask = 0x00ff0000,
  .Ashift = 24,
  .Amask = 0xff000000
};

/* LoadedTex is the type stored in the texture -> name hash table */
typedef struct {
  GLuint tex;
  char name[32];
  UT_hash_handle hh;
}LoadedTex;
static LoadedTex *loadedTextures = NULL;

/* renders the given character to a texture and returns its GL handle */
static GLuint latin1_to_texture(char c)
{
  static TTF_Font *font = NULL;
  static SDL_Color color = {.r = 200, .g = 255, .b = 255, .a = 128};
  char str[2];
  SDL_Surface *surf;
  LoadedTex *lup;
  GLuint tex;

  str[0] = c;
  str[1] = '\0';

  HASH_FIND_STR(loadedTextures, str, lup);
  if(lup != NULL){
    return lup->tex;
  }

  if(font == NULL){
    font = TTF_OpenFont("C64.ttf", 32);
  }
  if(font == NULL){
    printf("failed to load font: %s\n", TTF_GetError());
    return 0;
  }

  tex = -1;
  if((surf = TTF_RenderText_Solid(font, str, color))){
    int colors;
    SDL_Surface *optSurf;

    optSurf = SDL_ConvertSurface(surf, &pixelFmt, SDL_SWSURFACE);
    colors = surf->format->BytesPerPixel;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, optSurf->w, optSurf->h, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, optSurf->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(surf);
  }else{
    printf("error: failed to render char %c to texture: %s\n", c, TTF_GetError());
    tex = 0;
  }

  lup = malloc(sizeof(LoadedTex));

  lup->name[0] = c;
  lup->name[1] = '\0';
  lup->tex = tex;
  HASH_ADD_STR(loadedTextures, name, lup);

  return tex;
}

/* bitmap to texture loads file and returns a handle an OpenGL texture of it */
static GLuint bitmap_to_texture(const char *file)
{
  GLuint tex;
  SDL_Surface *surf;

  surf = NULL;
  tex = 0;
  if((surf = SDL_LoadBMP(file))){
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surf->w, surf->h, 0,
      GL_RGB, GL_UNSIGNED_BYTE, surf->pixels );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(surf);
  }else{
    printf("error: failed to load texture %s\n", file);
  }
  printf("%d\n", tex);
  return tex;
}

/* new_Runeset loads a texture file containings a page of runes */
uint32_t new_Runeset(const char *file)
{
  static uint32_t pages[MAX_RUNE_PAGES];
  static uint32_t numPages = 0;

  pages[numPages] = bitmap_to_texture(file);
  return pages[numPages++];
}

/* ctor */
Rune *new_Rune()
{
  Rune *r = malloc(sizeof(Rune));

  r->draw   = NULL;
  r->update = NULL;
  return r;
}

/* dtor */
void del_Rune(Rune* r)
{
  free(r);
}

/* rune_Draw executes r's draw method */
void rune_Draw(Rune *r, uint32_t x, uint32_t y)
{
  r->draw(r, x, y);
}

/* rune_DrawChar renders the given rune at char position (x, y) */
RuneDrawResult rune_DrawChar(Rune *rune, uint32_t x, uint32_t y)
{
  RuneDrawResult res;
  CharRune *r;
  GLfloat u, v;
  GLuint sampler;

  r = (CharRune*)rune;

  /* load texture */
  if(r->texture == 0){
    r->texture = latin1_to_texture('a'); //r->id);
    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

  }
  res.tex = r->texture;

  res.pos.x = 0.0f;
  res.pos.y = 0.0f;
  res.pos.w = rune->w;
  res.pos.h = rune->h;

  res.clip.x = 0.0f;
  res.clip.y = 0.0f;
  res.clip.w = 1.0f;
  res.clip.h = 1.0f;

  return res;
}

/* rune_DrawImg renders the given image rune */
RuneDrawResult rune_DrawImg(Rune *rune, uint32_t x, uint32_t y)
{
  RuneDrawResult res;
  ImgRune *r;
  GLfloat u, v;
  GLuint sampler;

  r = (ImgRune*)rune;

  /* load image as texture */
  if(r->texture == 0){
    r->texture = bitmap_to_texture(r->filename);
    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
  }
  res.tex = r->texture;

  res.pos.x = 0.0f;
  res.pos.y = 0.0f;
  res.pos.w = rune->w;
  res.pos.h = rune->h;

  res.clip.x = 0.0f;
  res.clip.y = 0.0f;
  res.clip.w = 1.0f;
  res.clip.h = 1.0f;

  return res;
}

/* rune_DrawMesh renders a mesh at char position (x, y) */
void rune_DrawMesh(MeshRune *r, uint32_t x, uint32_t y)
{
  /* do not render if this isn't the upper-left corner */
  if((r->x != 0) || (r->y != 0)){
    return;
  }
}

/* rune_Update executes r's update method */
void rune_Update(Rune *r)
{
  if(r->update != NULL){
    r->update(r);
  }
}

