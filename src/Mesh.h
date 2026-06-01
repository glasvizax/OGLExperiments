#pragma once
#include <cstddef>

#include <span>
#include <type_traits>

#include <glm/glm.hpp>

#include "Aliases.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

class Texture;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct VertexTB
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

#define _AUTO_ENABLE(pos, vtype, vfield)                                       \
	enableAttribute(                                                           \
		pos,                                                                   \
		decltype(vtype::vfield)::length(),                                     \
		stride,                                                                \
		offsetof(vtype, vfield) / sizeof(float)                                \
	)

template <>
inline void VertexArray::autoEnableAttributes<Vertex>()
{
	constexpr GLuint stride = sizeof(Vertex) / sizeof(float);

	_AUTO_ENABLE(0, Vertex, position);
	_AUTO_ENABLE(1, Vertex, normal);
	_AUTO_ENABLE(2, Vertex, uv);
}

template <>
inline void VertexArray::autoEnableAttributes<VertexTB>()
{
	constexpr GLuint stride = sizeof(VertexTB) / sizeof(float);

	_AUTO_ENABLE(0, VertexTB, position);
	_AUTO_ENABLE(1, VertexTB, normal);
	_AUTO_ENABLE(2, VertexTB, uv);
	_AUTO_ENABLE(3, VertexTB, tangent);
	_AUTO_ENABLE(4, VertexTB, bitangent);
}

struct Material
{
	std::string_view instance_name = "material";

	Texture* diffuse_map = nullptr;
	Texture* specular_map = nullptr;

	float shininess;

	void setShaderObject(ShaderProgram& program) const;
};

struct Light
{
	std::string_view instance_name = "light";

	glm::vec3 cam_position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void setShaderObject(ShaderProgram& program) const;
};

struct Mesh
{

public:
	template <typename VertexType>
	Mesh(std::span<VertexType> vertices);

	template <typename VertexType>
	Mesh(std::span<VertexType> vertices, std::span<glm::uvec3> indices);

	template <typename VertexType>
	Mesh(std::span<VertexType> vertices, std::span<uint> indices);

	void draw();

private:
	GLsizei m_vertices_count = 0;
	GLsizei m_indices_count = 0;

	bool m_draw_element = true;

	VertexArray vertex_array; // TODO : improve allocation
};

template <typename VertexType>
inline Mesh::Mesh(std::span<VertexType> vertices)
{
	m_vertices_count = vertices.size();
	m_draw_element = false;

	vertex_array.init();
	vertex_array.bind();
	// checkGeneralErrorGL("mesh");
	vertex_array.attachArrayBuffer(
		vertices.size() * sizeof(VertexType),
		vertices.data()
	);
	vertex_array.autoEnableAttributes<VertexType>();
	// checkGeneralErrorGL("mesh");
}

template <typename VertexType>
inline Mesh::Mesh(std::span<VertexType> vertices, std::span<uint> indices)
{
	m_vertices_count = vertices.size();
	m_indices_count = indices.size();
	m_draw_element = true;

	vertex_array.init();
	vertex_array.bind();

	// checkGeneralErrorGL("mesh");

	vertex_array.attachArrayBuffer(
		vertices.size() * sizeof(VertexType),
		vertices.data()
	);

	vertex_array.autoEnableAttributes<VertexType>();
	// checkGeneralErrorGL("mesh");

	vertex_array.attachElementBuffer(
		indices.size() * sizeof(uint),
		indices.data()
	);

	// checkGeneralErrorGL("mesh");
}

template <typename VertexType>
inline Mesh::Mesh(std::span<VertexType> vertices, std::span<glm::uvec3> indices)
	: Mesh(
		  vertices,
		  std::span<uint>(
			  reinterpret_cast<uint*>(indices.data()),
			  indices.size() * 3
		  )
	  )
{
}

// extern Mesh g_cube_mesh;