#pragma once
#include <cstddef>

#include <span>
#include <type_traits>

#include <glm/glm.hpp>

#include "Aliases.h"
#include "VertexArray.h"

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

#define _AUTO_ENABLE(pos, vtype, vfield) enableAttribute( \
		pos, \
		decltype(vtype::vfield)::length(), \
		stride, \
		offsetof(vtype, vfield) / sizeof(float) \
	)

template <>
inline void VertexArray::autoEnableAttributes<Vertex>()
{
	constexpr GLuint stride = sizeof(Vertex) / sizeof(float);

	_AUTO_ENABLE(0, Vertex, position);
	_AUTO_ENABLE(1, Vertex, normal);
	_AUTO_ENABLE(1, Vertex, uv);

	/*
	enableAttribute(
		0,
		decltype(Vertex::position)::length(),
		stride,
		offsetof(Vertex, position) / sizeof(float)
	);

	enableAttribute(
		1,
		decltype(Vertex::normal)::length(),
		stride,
		offsetof(Vertex, normal) / sizeof(float)
	);

	enableAttribute(
		2,
		decltype(Vertex::uv)::length(),
		stride,
		offsetof(Vertex, uv) / sizeof(float)
	);

	*/
}

template <>
inline void VertexArray::autoEnableAttributes<VertexTB>()
{
	constexpr GLuint stride = sizeof(VertexTB) / sizeof(float);

	_AUTO_ENABLE(0, VertexTB, position);
	_AUTO_ENABLE(1, VertexTB, normal);
	_AUTO_ENABLE(2, VertexTB, uv);
	_AUTO_ENABLE(3, VertexTB, tangent);
	_AUTO_ENABLE(3, VertexTB, bitangent);

	/*
	enableAttribute(
		0,
		vector_dim_v<decltype(VertexTB::position)>,
		stride,
		offsetof(VertexTB, position) / sizeof(float)
	);

	enableAttribute(
		1,
		vector_dim_v<decltype(VertexTB::normal)>,
		stride,
		offsetof(VertexTB, normal) / sizeof(float)
	);

	enableAttribute(
		2,
		vector_dim_v<decltype(VertexTB::uv)>,
		stride,
		offsetof(VertexTB, uv) / sizeof(float)
	);

	enableAttribute(
		3,
		vector_dim_v<decltype(VertexTB::tangent)>,
		stride,
		offsetof(VertexTB, tangent) / sizeof(float)
	);

	enableAttribute(
		4,
		vector_dim_v<decltype(VertexTB::bitangent)>,
		stride,
		offsetof(VertexTB, bitangent) / sizeof(float)
	);
	*/
}

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

//extern Mesh g_cube_mesh;