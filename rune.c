#include <malloc.h>
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
    GLuint fmt;
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
void rune_DrawChar(Rune *rune, uint32_t x, uint32_t y)
{
  const GLchar *vs = 
    "#version 150\n"
    "in vec2 pos;\n"
    "in vec2 texco;\n"
    "out vec2 out_texco;\n"
    "uniform sampler2D tex;\n"
    "uniform mat4 mvp;\n"
    "void main()\n"
    "{\n"
    "  out_texco = texco;\n"
    "  gl_Position = mvp * vec4(pos, 0.0, 1.0);\n"
    "}\n";
  const GLchar *fs = 
    "#version 150\n"
    "in vec2 out_texco;\n"
    "uniform sampler2D tex;\n"
    "void main()\n"
    "{\n"
    "  vec4 color = texture(tex, out_texco);\n"
    "  gl_FragColor = color;\n"
    "}\n";
  static GLuint vao = 0;
  static GLuint vbo = 0;
  static GLuint ibo = 0;
  static GLfloat vertices[4 * 4] = { /* 4 vertices. format: x-y-u-v */
    0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
  };
  static GLshort indices[3 * 2] = { /* 2 triangles */
    0, 1, 2,
    0, 3, 2
  };
  static Mat4x4 mvp;
  static GLuint mvpUniform;
  static GLuint texUniform;
  static GLuint shader;
  static GLuint sampler;

  GLenum err;
  GLint compiled, linked;
  CharRune *r;
  GLfloat u, v;

  r = (CharRune*)rune;

  /* load texture */
  if(r->texture == 0){
    r->texture = latin1_to_texture(r->id);
    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
  }

  /* create shader program */
  if(shader == 0){
    GLuint frag, vert;
    GLint len;

    /* vertex shader */
    vert = glCreateShader(GL_VERTEX_SHADER);
    len = strlen(vs);
    glShaderSource(vert, 1, &vs, &len);
    glCompileShader(vert);
    glGetShaderiv(vert, GL_COMPILE_STATUS, &compiled);
    if(compiled == GL_FALSE){
      GLint logSz;
      GLchar *log;

      glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &logSz);
      log = malloc(logSz * sizeof(GLchar));
      glGetShaderInfoLog(vert, logSz, &logSz, log);
      puts("error: vertex shader compilation failed");
      puts(log);
      free(log);
    }
    /* fragment shader */
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    len = strlen(fs);
    glShaderSource(frag, 1, &fs, &len);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &compiled);
    if(compiled == GL_FALSE){
      GLint logSz;
      GLchar *log;

      glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &logSz);
      log = malloc(logSz * sizeof(GLchar));
      glGetShaderInfoLog(vert, logSz, &logSz, log);
      puts("error: fragment shader compilation failed");
      puts(log);
      free(log);
    }

    /* link shaders */
    shader = glCreateProgram();
    glAttachShader(shader, vert);
    glAttachShader(shader, frag);
    glBindAttribLocation(shader, 0, "pos");
    glBindAttribLocation(shader, 1, "texco");
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &linked);
    if(linked == GL_FALSE){
      GLint logSz;
      GLchar *log;

      glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSz);
      log = malloc(logSz * sizeof(GLchar));
      glGetProgramInfoLog(shader, logSz, &logSz, log);
      puts(log);
      free(log);
    }
    /* get the uniforms */
    mvpUniform = glGetUniformLocation(shader, "mvp");
    mat4x4_orthographic(&mvp, 0.0f, 80.0f, 0.0f, 40.0f, -1.0f, 1.0f);
    texUniform = glGetUniformLocation(shader, "tex");
  }

  /* create vertex attribute object */
  if(vao == 0){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    /* vertices */
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4,
      vertices, GL_STATIC_DRAW);

    /* indices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * 6,
      indices, GL_STATIC_DRAW);
  }

  /* update VBO */
  r->id = 1;
  u = 1.0f / 16.0f * (r->id % 16);
  v = 1.0f / 16.0f * (r->id / 16);
  vertices[0] = x;
  vertices[1] = y;
  //vertices[2] = u;
  //vertices[3] = v;

  vertices[0+4] = x + rune->w;
  vertices[1+4] = y;
  //vertices[2+4] = u+(1.0f/16.0f);
  //vertices[3+4] = v;

  vertices[0+8] = x + rune->w;
  vertices[1+8] = y + rune->h;
  //vertices[2+8] = u+(1.0f/16.0f);
  //vertices[3+8] = v+(1.0f/16.0f);

  vertices[0+12] = x;
  vertices[1+12] = y + rune->h;
  //vertices[2+12] = u;
  //vertices[3+12] = v+(1.0f/16.0f);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4,
    vertices, GL_STATIC_DRAW);

  /* draw */
  glUseProgram(shader);
  glUniformMatrix4fv(mvpUniform, 1, 0, ((GLfloat*)&mvp));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, r->texture);
  glUniform1i(texUniform, 0);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)8);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
}

/* rune_DrawMesh renders a mesh at char position (x, y) */
void rune_DrawMesh(MeshRune *r, uint32_t x, uint32_t y)
{

}

/* rune_Update executes r's update method */
void rune_Update(Rune *r)
{
  if(r->update != NULL){
    r->update(r);
  }
}

