#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include "render.h"


/* if >0, the offset of the attribute in the vertex memory layout */
typedef struct {
  int pos;
  int color;
  int normal;
  int texco;
}MeshAttributes;

/* the standard vertex for meshes */
typedef struct {
  Position pos;
  Color    color;
  Normal   normal;
  Texco    texco;
}MeshVertex;

typedef struct {
  MeshAttributes attributes;

  MeshVertex *vertices;
  uint32_t numVertices;

  Face *faces;
  uint32_t numFaces;

  GLuint vao;
}Mesh;

Mesh * new_Mesh();
void   del_Mesh(Mesh*);

void mesh_Load(Mesh*, const char *);
void mesh_Draw(Mesh*);

#endif

