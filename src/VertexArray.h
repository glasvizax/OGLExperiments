#pragma once

#include <vector>

#include <glad/glad.h>

#include "ObjectID.h"

class VertexArray
{

public:
	void init();

	void attachArrayBuffer(GLsizeiptr bytes, const void* data, GLenum draw_mode = GL_STATIC_DRAW);
	void attachElementBuffer(GLsizeiptr bytes, const void* data, GLenum draw_mode = GL_STATIC_DRAW);

	void bind();

	void enableAttribute(GLuint index, GLint components_num, GLuint stride, GLsizei offset);

	void clear();

private:
	ObjectID<VertexArray> m_id;
	std::vector<GLuint> m_buffers;
};