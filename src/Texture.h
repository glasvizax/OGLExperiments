#pragma once

#include "Aliases.h"
#include "GLHandle.h"

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
	)
	{
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			internal_format,
			width,
			height,
			0,
			format,
			GL_UNSIGNED_BYTE,
			data
		);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR
		);

		glTexParameteri( //
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			GL_LINEAR
		);

		glTexParameteri( //
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			GL_CLAMP_TO_EDGE
		);

		glTexParameteri( //
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE
		);

		m_tex = TexHandle(id);
	}

	void bind(uint8 slot = 0)
	{
		GLenum gl_slot = GL_TEXTURE0 + slot;

		m_tex.bind(gl_slot);
	}

private:
	TexHandle m_tex;
};