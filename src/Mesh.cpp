#include "Mesh.h"

#include "Texture.h"

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

void Material::setShaderObject(ShaderProgram& program) const
{
	std::string_view diffuse_field[2] = {instance_name, ".diffuse_map"};
	std::string_view specular_field[2] = {instance_name, ".specular_map"};
	std::string_view shininess_field[2] = {instance_name, ".shininess"};

	uint32 diffuse_hash = hashString(diffuse_field);
	uint32 specular_hash = hashString(specular_field);
	uint32 shininess_hash = hashString(shininess_field);

	program.setObject(*diffuse_map, diffuse_hash, 0u);
	program.setObject(*specular_map, specular_hash, 1u);
	program.set(shininess_hash, shininess);
}

void Light::setShaderObject(ShaderProgram& program) const
{
	std::string_view ambient_field[2] = {instance_name, ".ambient"};
	std::string_view diffuse_field[2] = {instance_name, ".diffuse"};
	std::string_view specular_field[2] = {instance_name, ".specular"};
	std::string_view cam_position_field[2] = {instance_name, ".cam_position"};

	uint32 ambient_hash = hashString(ambient_field);
	uint32 diffuse_hash = hashString(diffuse_field);
	uint32 specular_hash = hashString(specular_field);
	uint32 cam_position_hash = hashString(cam_position_field);

	program.setVec(ambient_hash, ambient);
	program.setVec(diffuse_hash, diffuse);
	program.setVec(specular_hash, specular);
	program.setVec(cam_position_hash, cam_position);
}