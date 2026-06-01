#include "Texture.h"

void Texture::init(
	GLint internal_format,
	GLsizei width,
	GLsizei height,
	GLenum format,
	uchar* data
)
{
	GLuint id;
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
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

void Texture::bind(uint slot) const
{
	GLenum gl_slot = GL_TEXTURE0 + slot;
	m_tex.bind(gl_slot);
}

void Texture::setShaderObject(
	ShaderProgram& shader_program,
	uint32 hash_name,
	uint slot
) const
{
	shader_program.set(hash_name, static_cast<GLint>(slot));
	bind(slot);
}
