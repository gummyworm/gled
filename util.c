#include "util.h"

GLuint loadShader(const GLchar *vs, const GLchar *fs, int numAttrs,
		  const char **attrs) {
	GLint compiled, linked;
	GLuint shader;
	GLuint frag, vert;
	GLint len;
	int i;

	/* vertex shader */
	vert = glCreateShader(GL_VERTEX_SHADER);
	len = strlen(vs);
	glShaderSource(vert, 1, &vs, &len);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
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
	if (compiled == GL_FALSE) {
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

	for (i = 0; i < numAttrs; ++i) {
		glBindAttribLocation(shader, i, attrs[i]);
	}
	glBindAttribLocation(shader, 1, "texco");

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE) {
		GLint logSz;
		GLchar *log;

		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSz);
		log = malloc(logSz * sizeof(GLchar));
		glGetProgramInfoLog(shader, logSz, &logSz, log);
		puts(fs);
		puts(log);
		free(log);
	}

	return shader;
}
