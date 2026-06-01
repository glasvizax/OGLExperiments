#pragma once
#include "glm/glm.hpp"

#include "ShaderProgram.h"

namespace
{
#include "ShaderUniformFuncHelper.h"
} // namespace

template <size_t N, typename T>
inline void ShaderProgram::setVec(uint32 hash_name, glm::vec<N, T> vec)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");
	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double> ||
			std::is_same_v<T, int> || std::is_same_v<T, uint> ||
			std::is_same_v<T, size_t> || std::is_same_v<T, bool>,
		"T must be float, double, int or uint"
	);

	bind();
	// checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation(hash_name);
	// checkGeneralErrorGL(m_debug_name);
	uniformVecFunc<N, T>(location, vec);
	// checkGeneralErrorGL(m_debug_name);
}

template <size_t N, typename T>
inline void ShaderProgram::setVecArray(
	uint32 hash_name,
	const glm::vec<N, T>* const array,
	GLsizei count
)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");
	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double> ||
			std::is_same_v<T, int> || std::is_same_v<T, uint> ||
			std::is_same_v<T, size_t> || std::is_same_v<T, bool>,
		"T must be float, double, int or uint"
	);

	bind();
	// checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation(hash_name);
	uniformVecArrayFunc<N, T>(location, array, count);
	// checkGeneralErrorGL(m_debug_name);
}

template <typename T>
inline void ShaderProgram::set(uint32 hash_name, T val)
{
	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double> ||
			std::is_same_v<T, int> || std::is_same_v<T, uint> ||
			std::is_same_v<T, size_t> || std::is_same_v<T, bool>,
		"T must be float, double, int uint or size_t"
	);

	bind();
	// checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation(hash_name);
	uniformValueFunc<T>(location, val);
	// checkGeneralErrorGL(m_debug_name);
}

template <typename T>
inline void
ShaderProgram::setArray(uint32 hash_name, const T* const vals, GLsizei count)
{
	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double> ||
			std::is_same_v<T, int> || std::is_same_v<T, uint> ||
			std::is_same_v<T, size_t> || std::is_same_v<T, bool>,
		"T must be float, double, int or uint"
	);

	bind();
	// checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation(hash_name);
	uniformValueArrayFunc<T>(location, vals, count);
	// checkGeneralErrorGL(m_debug_name);
}

template <size_t N, typename T>
inline void
ShaderProgram::setMat(uint32 hash_name, const glm::mat<N, N, T>& mat)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");

	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double>,
		"T must be float, double"
	);

	bind();
	// checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation(hash_name);
	uniformMatArrayFunc<N, T>(location, &mat, 1);
	// checkGeneralErrorGL(m_debug_name);
}

template <size_t N, typename T>
inline void ShaderProgram::setMatArray(
	uint32 hash_name,
	const glm::mat<N, N, T>* const array,
	GLsizei count
)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");

	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double>,
		"T must be float, double, int or uint"
	);

	bind();

	GLint location = getLocation(hash_name);
	// checkGeneralErrorGL(m_debug_name);
	uniformMatArrayFunc<N, T>(location, array, count);
	// checkGeneralErrorGL(m_debug_name);
}

