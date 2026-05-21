#include <string>
#include <string_view>

#include "ShaderProgram.h"

ShaderProgram initLoadShaderProgram(
	std::string_view vertex_name,
	std::string_view fragment_name
);

ShaderProgram initLoadShaderProgram(
	std::string_view vertex_name,
	std::string_view fragment_name,
	std::string_view geometry_name
);

bool readFile(const std::filesystem::path& path, std::string& content);