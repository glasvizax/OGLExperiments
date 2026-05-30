#pragma once

#pragma once

#include <string>
#include <type_traits>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Aliases.h"
#include "GLHandle.h"
#include "Misc.h"

struct ShaderProgramTrait
{
	static void destroy(GLuint id)
	{
		glDeleteProgram(id);
	}

	static void bind(GLuint id)
	{
		glUseProgram(id);
	}
};

using ShaderProgramHandle = GLHandle<ShaderProgramTrait>;

class ShaderProgram
{

public:
	ShaderProgram() noexcept = default;

	bool init(const char* const vertex_src, const char* const fragment_src);
	bool init(
		const char* const vertex_src,
		const char* const fragment_src,
		const char* const geometry_src
	);

	void bind() const;

	template <size_t N, typename T>
	void setVec(uint32 hash_name, glm::vec<N, T> vec);

	template <size_t N, typename T>
	void setVecArray(
		uint32 hash_name,
		const glm::vec<N, T>* const array,
		GLsizei count
	);

	template <typename T>
	void set(uint32 hash_name, T val);

	template <typename T>
	void setArray(uint32 hash_name, const T* const vals, GLsizei count);

	template <size_t N, typename T>
	void setMat(uint32 hash_name, const glm::mat<N, N, T>& mat);

	template <size_t N, typename T>
	void setMatArray(
		uint32 hash_name,
		const glm::mat<N, N, T>* const array,
		GLsizei count
	);

private:
	GLint getLocation(uint32 hash_name);
	void reflectUniforms();

	ShaderProgramHandle m_program;
	std::vector<std::pair<uint32, GLint>> m_locations;
};

#include "ShaderSetFuncImpl.h"