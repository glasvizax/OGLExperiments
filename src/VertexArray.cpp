#include "VertexArray.h"

void VertexArray::init()
{	
	glGenVertexArrays(1, &m_id.id);
}

void VertexArray::attachArrayBuffer(GLsizeiptr bytes, const void* data, GLenum draw_mode)
{
	bind();
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	m_buffers.push_back(vbo);
	glBufferData(GL_ARRAY_BUFFER, bytes, data, draw_mode);
}

void VertexArray::attachElementBuffer(GLsizeiptr bytes, const void* data, GLenum draw_mode)
{
	bind();
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	m_buffers.push_back(ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes, data, draw_mode);
}

VertexArray::~VertexArray()
{
	clear();
}

void VertexArray::bind()
{
	if (m_id.bind()) 
	{
		glBindVertexArray(m_id.id);
	}
}

void VertexArray::enableAttribute(GLuint index, GLint components_num, GLuint stride, GLsizei offset)
{
	bind();
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, components_num, GL_FLOAT, GL_FALSE, stride * sizeof(float), reinterpret_cast<void*>(sizeof(float) * offset));
}

void VertexArray::clear()
{
	if (m_id.id)
	{
		glDeleteVertexArrays(1, &m_id.id);
	}

	for (GLuint buffer : m_buffers)
	{
		glDeleteBuffers(1, &buffer);
	}

	m_id.clear();
	m_buffers.clear();
}