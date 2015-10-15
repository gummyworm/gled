#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <malloc.h>
#include <stdint.h>
#include "mesh.h"

Mesh * new_Mesh()
{
  Mesh *m;

  m = malloc(sizeof(Mesh));

  m->vertices = NULL;
  return m;
}

void del_Mesh(Mesh *m)
{
  if(m->vertices != NULL){
    free(m->vertices);
  }
  free(m);
}

/* mesh_Load loads m with the mesh described by filename */
void mesh_Load(Mesh *m, const char *filename)
{
  unsigned int i;
  struct aiMesh *iMesh;
  const struct aiScene * scene = aiImportFile(filename,
                  aiProcess_CalcTangentSpace      |
                  aiProcess_Triangulate           |
                  aiProcess_JoinIdenticalVertices |
                  aiProcess_SortByPType);

  if(scene == NULL){
    return;
  }
  if(scene->mNumMeshes == 0){
    return;
  }

  /* get the vertices */
  m->vertices = malloc(sizeof(MeshVertex) * scene->mMeshes[0]->mNumVertices);
  for(i = 0, iMesh = scene->mMeshes[0]; i < iMesh->mNumVertices; ++i){
    m->vertices[i].pos[0] = iMesh->mVertices[i].x;
    m->vertices[i].pos[1] = iMesh->mVertices[i].y;
    m->vertices[i].pos[2] = iMesh->mVertices[i].z;

    if(iMesh->mNormals){
      m->vertices[i].normal[0] = iMesh->mNormals[i].x;
      m->vertices[i].normal[1] = iMesh->mNormals[i].y;
      m->vertices[i].normal[2] = iMesh->mNormals[i].z;
    }else{
      m->vertices[i].normal[0] =
      m->vertices[i].normal[1] =
      m->vertices[i].normal[2] =
      m->vertices[i].normal[3] = 0.0f;
    }

    if(iMesh->mTextureCoords){
      m->vertices[i].texco[0] = iMesh->mTextureCoords[i][0].x;
      m->vertices[i].texco[1] = iMesh->mTextureCoords[i][0].y;
    }else{
      m->vertices[i].texco[0] =
      m->vertices[i].texco[1] =
      m->vertices[i].texco[2] =
      m->vertices[i].texco[3] = 0.0f;
    }

    if(iMesh->mColors){
      m->vertices[i].color[0] = iMesh->mColors[i][0].r;
      m->vertices[i].color[1] = iMesh->mColors[i][0].g;
      m->vertices[i].color[2] = iMesh->mColors[i][0].b; 
      m->vertices[i].color[3] = iMesh->mColors[i][0].a;
    }else{
      m->vertices[i].color[0] =
      m->vertices[i].color[1] =
      m->vertices[i].color[2] =
      m->vertices[i].color[3] = 0.0f;
    }
  }
  m->numVertices = iMesh->mNumVertices;

  /* get the indices for the faces */
  for(i = 0; i < iMesh->mNumFaces; ++i){
    m->faces[i][0]  = iMesh->mFaces[i].mIndices[0];
    m->faces[i][1]  = iMesh->mFaces[i].mIndices[1];
    m->faces[i][2]  = iMesh->mFaces[i].mIndices[2];
  }
  m->numFaces = iMesh->mNumFaces;

  glGenBuffers(1, &m->vao);
  glBindBuffer(GL_ARRAY_BUFFER, m->vao);
  glBufferData(GL_ARRAY_BUFFER,
                  sizeof(MeshVertex) * iMesh->mNumVertices,
                  m->vertices,
                  GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* mesh_Draw renders mesh m. */
void mesh_Draw(Mesh *m)
{
  glBindBuffer(GL_ARRAY_BUFFER, m->vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
          (GLvoid*)(intptr_t)m->attributes.pos);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
          (GLvoid*)(intptr_t)m->attributes.color);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
          (GLvoid*)(intptr_t)m->attributes.normal);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
          (GLvoid*)(intptr_t)m->attributes.texco);

  glDrawElements(GL_TRIANGLES, m->numFaces, GL_UNSIGNED_SHORT, m->faces);
}

