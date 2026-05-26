#include "Mesh.h"

void Mesh::draw()
{
	vertex_array.bind();
	if (m_draw_element)
	{
		glDrawElements(GL_TRIANGLES, m_indices_count, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_vertices_count);
	}
	// checkGeneralErrorGL("mesh");
}



/*
Mesh g_cube_mesh = Mesh(
	std::span<Vertex>(g_cube_vertices),
	std::span<glm::uvec3>(g_cube_indices)
);*/