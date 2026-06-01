#pragma once

#include "Aliases.h"
#include "GLHandle.h"
#include "ShaderProgram.h"

class GLTextureHandleCache
{
public:
	static bool isBound(GLuint id)
	{
		return false;
	}

	static void reset() {}

	void update(GLuint id, GLenum slot) {}
};

struct TextureTrait
{
	static void destroy(GLuint id)
	{
		glDeleteTextures(1, &id);
	}

	static void bind(GLuint id, GLenum slot)
	{
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
};

using TexHandle = GLHandle<TextureTrait, GLTextureHandleCache>;

class Texture
{
public:
	void init(
		GLint internal_format,
		GLsizei width,
		GLsizei height,
		GLenum format,
		uchar* data
	);

	void bind(uint slot = 0) const;

	void setShaderObject(
		ShaderProgram& shader_program,
		uint32 hash_name,
		uint slot
	) const;

private:
	mutable TexHandle m_tex;
};

