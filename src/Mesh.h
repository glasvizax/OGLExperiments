#pragma once
#include <cstddef>

#include <span>
#include <type_traits>

#include <xm/xm.h>

#include "Aliases.h"
#include "VertexArray.h"

struct Vertex
{
	xm::vec3 position;
	xm::vec3 normal;
	xm::vec2 uv;
};

struct VertexTB
{
	xm::vec3 position;
	xm::vec3 normal;
	xm::vec2 uv;
	xm::vec3 tangent;
	xm::vec3 bitangent;
};

template <typename T>
struct vector_dim : std::integral_constant<uint8, 0>
{
};

template <uint8 N, typename T>
struct vector_dim<xm::vector<N, T>> : std::integral_constant<uint8, N>
{
};

template <typename T>
constexpr uint8 vector_dim_v = vector_dim<T>::value;

template <>
inline void VertexArray::autoEnableAttributes<Vertex>()
{
	constexpr GLuint stride = sizeof(Vertex) / sizeof(float);

	enableAttribute(
		0,
		vector_dim_v<decltype(Vertex::position)>,
		stride,
		offsetof(Vertex, position) / sizeof(float)
	);

	enableAttribute(
		1,
		vector_dim_v<decltype(Vertex::normal)>,
		stride,
		offsetof(Vertex, normal) / sizeof(float)
	);

	enableAttribute(
		2,
		vector_dim_v<decltype(Vertex::uv)>,
		stride,
		offsetof(Vertex, uv) / sizeof(float)
	);
}

template <>
inline void VertexArray::autoEnableAttributes<VertexTB>()
{
	constexpr GLuint stride = sizeof(VertexTB) / sizeof(float);

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
}

struct Mesh
{

public:
	template <typename VertexType>
	Mesh(std::span<VertexType> vertices);

	template <typename VertexType>
	Mesh(std::span<VertexType> vertices, std::span<xm::uvec3> indices);

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
inline Mesh::Mesh(std::span<VertexType> vertices, std::span<xm::uvec3> indices)
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