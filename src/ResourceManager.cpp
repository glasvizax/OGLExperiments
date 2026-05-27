#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <span>
#include <vector>

#include <stb_image.h>

#include "Aliases.h"
#include "GlobalPaths.h"

static std::vector<std::pair<uint32, Texture>> s_textures;

void ResourceManager::init()
{
	s_textures.reserve(32);
}

ShaderProgram ResourceManager::initLoadShaderProgram(
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

Texture* ResourceManager::initLoadTexture(std::string_view filename)
{
	std::string path = g_content_path + std::string(filename);
	uint32 hash = hashString(path);
	auto it = std::find_if(
		s_textures.begin(),
		s_textures.end(),
		[hash](auto& pair) 
		{ // 
			return pair.first == hash; 
		}
	);

	if (it != s_textures.end())
	{
		return &it->second;
	}

	int width, height, channels;
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		std::cerr << "coudln't load texture: " << path << std::endl;
		assert(false);
		return nullptr;
	}

	GLint format;
	switch (channels)
	{
		case (1):
		{
			format = GL_RED;
			break;
		}
		case (2):
		{
			format = GL_RG;
			break;
		}
		case (3):
		{
			format = GL_RGB;
			break;
		}
		case (4):
		{
			format = GL_RGBA;
			break;
		}
		default:
		{
			break;
		}
	}

	auto& pair = s_textures.emplace_back();
	pair.first = hash;
	pair.second.init(format, width, height, format, data);
	return &pair.second;
}

bool ResourceManager::readFile(
	const std::filesystem::path& path,
	std::string& content
)
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