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

void setMaterial(
	std::string_view instance_name,
	const Material& material,
	ShaderProgram& shader_program
)
{
	std::string_view ambient_field[2] = {instance_name, ".ambient"};
	std::string_view diffuse_field[2] = {instance_name, ".diffuse"};
	std::string_view specular_field[2] = {instance_name, ".specular"};
	std::string_view shininess_field[2] = {instance_name, ".shininess"};

	uint32 ambient_hash = hashString(ambient_field);
	uint32 diffuse_hash = hashString(diffuse_field);
	uint32 specular_hash = hashString(specular_field);
	uint32 shininess_hash = hashString(shininess_field);

	shader_program.setVec(ambient_hash, material.ambient);
	shader_program.setVec(diffuse_hash, material.diffuse);
	shader_program.setVec(specular_hash, material.specular);
	shader_program.set(shininess_hash, material.shininess);
}

void setLight(
	std::string_view instance_name,
	const Light& light,
	ShaderProgram& shader_program
)
{
	std::string_view ambient_field[2] = {instance_name, ".ambient"};
	std::string_view diffuse_field[2] = {instance_name, ".diffuse"};
	std::string_view specular_field[2] = {instance_name, ".specular"};
	std::string_view cam_position_field[2] = {instance_name, ".cam_position"};

	uint32 ambient_hash = hashString(ambient_field);
	uint32 diffuse_hash = hashString(diffuse_field);
	uint32 specular_hash = hashString(specular_field);
	uint32 cam_position_hash = hashString(cam_position_field);

	shader_program.setVec(ambient_hash, light.ambient);
	shader_program.setVec(diffuse_hash, light.diffuse);
	shader_program.setVec(specular_hash, light.specular);
	shader_program.setVec(cam_position_hash, light.cam_position);
}
