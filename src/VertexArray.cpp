#include "VertexArray.h"

void VertexArray::init()
{
	GLuint id;
	glGenVertexArrays(1, &id);
	m_vao = VAOHandle(id);
}

void VertexArray::attachArrayBuffer(
	GLsizeiptr bytes,
	const void* data,
	GLenum draw_mode
)
{
	bind();
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bytes, data, draw_mode);
	m_vbos.emplace_back(vbo);
}

void VertexArray::attachElementBuffer(
	GLsizeiptr bytes,
	const void* data,
	GLenum draw_mode
)
{
	bind();
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes, data, draw_mode);
	m_ebo = EBOHandle(ebo);
}

void VertexArray::bind()
{
	m_vao.bind();
}

void VertexArray::enableAttribute(
	GLuint index,
	GLint components_num,
	GLuint stride,
	GLsizei offset
)
{
	bind();
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(
		index,
		components_num,
		GL_FLOAT,
		GL_FALSE,
		stride * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * offset)
	);
}
