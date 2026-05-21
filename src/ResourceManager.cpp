#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <span>

#include "GlobalPaths.h"

ShaderProgram initLoadShaderProgram(
	std::string_view vertex_name,
	std::string_view fragment_name
)
{
	ShaderProgram program;

	std::string vertex_src, fragment_src;
	std::filesystem::path current = g_shaders_path;
	if (!readFile(current.append(vertex_name), vertex_src))
	{
		std::cerr << "couldn't load vertex shader";
		return program;
	}

	current = g_shaders_path;
	if (!readFile(current.append(fragment_name), fragment_src))
	{
		std::cerr << "couldn't load vertex shader";
		return program;
	}

	program.init(vertex_src.c_str(), fragment_src.c_str());
	return program;
}

bool readFile(const std::filesystem::path& path, std::string& content)
{
	std::ifstream file(path, std::ios_base::binary);

	if (!file.is_open())
	{
		std::cerr << "couldn't open file: " << path.generic_string().c_str()
				  << std::endl;
		// LOG_ERROR_F("couldn't open file %s", path.generic_string().c_str());
		return false;
	}

	uintmax_t sz = fs::file_size(path);
	content.resize(sz);

	file.read(content.data(), sz);
	content[sz] = '\0';
	return true;
}