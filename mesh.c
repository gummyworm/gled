#include "mesh.h"
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdint.h>
#include <stdlib.h>
#include "matrix.h"
#include "util.h"

static const GLchar *vs =
    "#version 150\n"
    "in vec3 pos;\n"
    "in vec4 normal;\n"
    "in vec4 color;\n"
    "in vec2 texco;\n"
    "out vec4 out_co;\n"
    "uniform sampler2D tex;\n"
    "uniform mat4 mv, proj;\n"
    "void main()\n"
    "{\n"
    "  out_co = color;\n"
    "  gl_Position = proj * mv * vec4(pos, 1.0);\n"
    "}\n";

static const GLchar *fs =
    "#version 150\n"
    "in vec4 out_co;\n"
    "out vec4 out_color;\n"
    "void main()\n"
    "{\n"
    "  out_color = out_co;\n"
    "}\n";

void init_Mesh(Mesh *m) {
	m->vertices = NULL;

	/* RGBA8 2D texture, 24 bit depth texture, 256x256 */
	glGenTextures(1, &m->color);
	glBindTexture(GL_TEXTURE_2D, m->color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_RGBA,
		     GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &m->depth);
	glBindRenderbuffer(GL_RENDERBUFFER, m->depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 256, 256);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &m->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			       GL_TEXTURE_2D, m->color, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				  GL_RENDERBUFFER, m->depth);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FBO setup failed\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Mesh *new_Mesh() {
	Mesh *m;
	m = malloc(sizeof(Mesh));
	init_Mesh(m);
	return m;
}

void del_Mesh(Mesh *m) {
	if (m->vertices != NULL) {
		free(m->vertices);
	}

	glDeleteTextures(1, &m->depth);
	glDeleteFramebuffers(1, &m->fbo);
	free(m);
}

/* mesh_Load loads m with the mesh described by filename */
void mesh_Load(Mesh *m, const char *filename) {
	unsigned int i;
	struct aiMesh *iMesh;
	const struct aiScene *scene = aiImportFile(
	    filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
			  aiProcess_JoinIdenticalVertices |
			  aiProcess_SortByPType);
	bool hasColors, hasTexcos, hasNormals;

	if (scene == NULL) {
		return;
	}
	if (scene->mNumMeshes == 0) {
		return;
	}

	iMesh = scene->mMeshes[0];
	m->numVertices = iMesh->mNumVertices;
	m->vertices = malloc(sizeof(MeshVertex) * m->numVertices);

	hasNormals = iMesh->mNormals != NULL;
	hasColors = iMesh->mColors[0] != NULL;
	hasTexcos = iMesh->mTextureCoords[0] != NULL;

	/* get the vertices */
	for (i = 0; i < iMesh->mNumVertices; ++i) {
		m->vertices[i].pos[0] = iMesh->mVertices[i].x;
		m->vertices[i].pos[1] = iMesh->mVertices[i].y;
		m->vertices[i].pos[2] = iMesh->mVertices[i].z;

		if (hasNormals) {
			m->vertices[i].normal[0] = iMesh->mNormals[i].x;
			m->vertices[i].normal[1] = iMesh->mNormals[i].y;
			m->vertices[i].normal[2] = iMesh->mNormals[i].z;
		} else {
			m->vertices[i].normal[0] = m->vertices[i].normal[1] =
			    m->vertices[i].normal[2] = 0.0f;
		}

		if (hasTexcos) {
			m->vertices[i].texco[0] = iMesh->mTextureCoords[i][0].x;
			m->vertices[i].texco[1] = iMesh->mTextureCoords[i][0].y;
		} else {
			m->vertices[i].texco[0] = m->vertices[i].texco[1] =
			    0.0f;
		}

		if (hasColors) {
			m->vertices[i].color[0] = iMesh->mColors[i][0].r;
			m->vertices[i].color[1] = iMesh->mColors[i][0].g;
			m->vertices[i].color[2] = iMesh->mColors[i][0].b;
			m->vertices[i].color[3] = iMesh->mColors[i][0].a;
		} else {
			m->vertices[i].color[0] = m->vertices[i].color[1] =
			    m->vertices[i].color[2] = 0.5f;
			m->vertices[i].color[3] = 1.0f;
		}
	}

	/* get the indices for the faces */
	m->faces = malloc(sizeof(Face) * m->numFaces);
	for (i = 0; i < iMesh->mNumFaces; ++i) {
		m->faces[i][0] = iMesh->mFaces[i].mIndices[0];
		m->faces[i][1] = iMesh->mFaces[i].mIndices[1];
		m->faces[i][2] = iMesh->mFaces[i].mIndices[2];
	}
	m->numFaces = iMesh->mNumFaces;
	printf("%d faces\n", m->numFaces);

	glGenVertexArrays(1, &m->vao);
	glGenBuffers(1, &m->vbo);
	glGenBuffers(1, &m->ibo);

	glBindVertexArray(m->vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * m->numVertices,
		     m->vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
			      (GLvoid *)offsetof(MeshVertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
			      (GLvoid *)offsetof(MeshVertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
			      (GLvoid *)offsetof(MeshVertex, color));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
			      (GLvoid *)offsetof(MeshVertex, texco));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * m->numFaces,
		     m->faces, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);
}

/* mesh_Draw renders mesh m. */
void mesh_Draw(Mesh *m) {
	static Mat4x4 mv, proj;
	static GLuint mvUniform, projUniform;
	static GLuint shader;
	GLint vp[4];

	if (m->color == 0 || m->fbo == 0) {
		return;
	}

	/* create shader program */
	if (shader == 0) {
		const char *attrs[31] = {"pos", "normal", "color", "texco"};
		shader = loadShader(vs, fs, 4, attrs);

		/* get the uniforms */
		mvUniform = glGetUniformLocation(shader, "mv");
		projUniform = glGetUniformLocation(shader, "proj");
		mat4x4_perspective(&proj, 45.0f, 640.0f / 480.0f, 0.01f,
				   1000.0f);
		mat4x4_load_identity(&mv);
		mat4x4_translate(&mv, 0.0f, 0.0f, -3.0f);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m->fbo);
	glGetIntegerv(GL_VIEWPORT, vp);
	glViewport(0, 0, 256, 256);

	glUseProgram(shader);
	glUniformMatrix4fv(mvUniform, 1, GL_FALSE, ((GLfloat *)&mv));
	glUniformMatrix4fv(projUniform, 1, GL_FALSE, ((GLfloat *)&proj));

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindBuffer(GL_ARRAY_BUFFER, m->vao);
	glDrawElements(GL_TRIANGLES, m->numFaces * 3, GL_UNSIGNED_SHORT,
		       (void *)0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(vp[0], vp[1], vp[2], vp[3]);
	glDisable(GL_DEPTH_TEST);
	return;
}
