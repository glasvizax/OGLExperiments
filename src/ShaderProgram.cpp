#include "ShaderProgram.h"
#include "Aliases.h"

GLint ShaderProgram::getLocation(uint32 hash_name)
{
	for (const auto& [hash, loc] : m_locations)
	{
		if (hash == hash_name)
		{
			return loc;
		}
	}
	return -1;
}

bool setupShader(const char* const src, GLenum shader_type, GLuint& result);
bool setupProgram(GLuint* shaders, uint count, GLuint& program);

bool ShaderProgram::init(
	const char* const vertex_src,
	const char* const fragment_src
)
{
	if (!vertex_src)
	{
		// LOG_ERROR_S("Invalid vertex source");
		return false;
	}
	if (!fragment_src)
	{
		// LOG_ERROR_S("Invalid fragment source");
		return false;
	}

	GLuint shaders[2];

	if (!setupShader(vertex_src, GL_VERTEX_SHADER, shaders[0]))
	{
		return false;
	}
	if (!setupShader(fragment_src, GL_FRAGMENT_SHADER, shaders[1]))
	{
		glDeleteShader(shaders[0]);
		return false;
	}

	GLuint program;

	if (!setupProgram(shaders, 2, program))
	{
		return false;
	}

	m_program = ShaderProgramHandle(program);
	reflectUniforms();
	return true;
}

bool ShaderProgram::init(
	const char* const vertex_src,
	const char* const fragment_src,
	const char* const geometry_src
)
{
	if (!vertex_src)
	{
		// LOG_ERROR_S("Invalid vertex source");
		return false;
	}
	if (!fragment_src)
	{
		// LOG_ERROR_S("Invalid fragment source");
		return false;
	}
	if (!geometry_src)
	{
		//("Invalid geometry source");
		return false;
	}

	GLuint shaders[3];
	if (!setupShader(vertex_src, GL_VERTEX_SHADER, shaders[0]))
	{
		return false;
	}

	if (!setupShader(fragment_src, GL_FRAGMENT_SHADER, shaders[1]))
	{
		glDeleteShader(shaders[0]);
		return false;
	}

	if (!setupShader(geometry_src, GL_GEOMETRY_SHADER, shaders[2]))
	{
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		return false;
	}

	GLuint program;

	if (!setupProgram(shaders, 3, program))
	{
		return false;
	}

	m_program = ShaderProgramHandle(program);

	reflectUniforms();
	return true;
}

void ShaderProgram::bind() const
{
	m_program.bind();
}

bool setupShader(const char* const src, GLenum shader_type, GLuint& result)
{
	result = glCreateShader(shader_type);
	glShaderSource(result, 1, &src, nullptr);

	glCompileShader(result);

	GLint is_compiled;
	glGetShaderiv(result, GL_COMPILE_STATUS, &is_compiled);
	if (is_compiled == GL_FALSE)
	{
		GLint log_length;
		glGetShaderiv(result, GL_INFO_LOG_LENGTH, &log_length);

		std::string log;
		log.resize(log_length);
		glGetShaderInfoLog(result, log_length, nullptr, log.data());

		// LOG_ERROR_F("couldn't compile shader [%s]", log.c_str());

		glDeleteShader(result);
		return false;
	}
	return true;
}

bool setupProgram(GLuint* shaders, uint count, GLuint& program)
{
	program = glCreateProgram();

	for (uint i = 0; i < count; ++i)
	{
		glAttachShader(program, shaders[i]);
	}
	glLinkProgram(program);

	for (uint i = 0; i < count; ++i)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	GLint is_linked;
	glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
	if (is_linked == GL_FALSE)
	{
		const GLsizei buf_size = 512;
		char log[buf_size];
		glGetProgramInfoLog(program, buf_size, nullptr, log);
		// LOG_ERROR_F("couldn't link program [%s]", log.c_str());

		glDeleteProgram(program);

		for (uint i = 0; i < count; ++i)
		{
			glDetachShader(program, shaders[i]);
			glDeleteShader(shaders[i]);
		}

		return false;
	}

	return true;
}

void ShaderProgram::reflectUniforms()
{
	GLint count;
	glGetProgramiv(m_program.get(), GL_ACTIVE_UNIFORMS, &count);

	m_locations.reserve(count);

	const GLsizei buf_size = 256;
	GLchar name[buf_size];
	GLsizei length;
	GLint size;
	GLenum type;

	for (GLuint i = 0; i < count; i++)
	{
		glGetActiveUniform(
			m_program.get(),
			i,
			buf_size,
			&length,
			&size,
			&type,
			name
		);

		uint32 hash_name = hashString(name, length);

		GLint location = glGetUniformLocation(m_program.get(), name);

		m_locations.emplace_back(hash_name, location);
	}
}