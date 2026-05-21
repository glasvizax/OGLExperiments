#pragma once

#include <vector>

#include <glad/glad.h>

#include "GLHandle.h"

struct VertexArrayTrait
{
	static void destroy(GLuint id)
	{
		glDeleteVertexArrays(1, &id);
	}

	static void bind(GLuint id)
	{
		glBindVertexArray(id);
	}
};

struct ArrayBufferTrait
{
	static void destroy(GLuint id)
	{
		glDeleteBuffers(1, &id);
	}
	static void bind(GLuint id)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
};

struct ElementBufferTrait
{
	static void destroy(GLuint id)
	{
		glDeleteBuffers(1, &id);
	}
	static void bind(GLuint id)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
};

using VAOHandle = GLHandle<VertexArrayTrait>;
using VBOHandle = GLHandle<ArrayBufferTrait>;
using EBOHandle = GLHandle<ElementBufferTrait>;

class VertexArray
{

public:
	void init();

	void attachArrayBuffer(
		GLsizeiptr bytes,
		const void* data,
		GLenum draw_mode = GL_STATIC_DRAW
	);
	void attachElementBuffer(
		GLsizeiptr bytes,
		const void* data,
		GLenum draw_mode = GL_STATIC_DRAW
	);

	template <typename VertexType>
	void autoEnableAttributes();

	void bind();

	void enableAttribute(
		GLuint index,
		GLint components_num,
		GLuint stride,
		GLsizei offset
	);

private:
	VAOHandle m_vao;
	std::vector<VBOHandle> m_vbos;
	EBOHandle m_ebo;
};