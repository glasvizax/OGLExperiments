#include <string>
#include <string_view>

#include "ShaderProgram.h"
#include "Texture.h"

class ResourceManager
{
public:
	void init();

	ShaderProgram initLoadShaderProgram(
		std::string_view vertex_name,
		std::string_view fragment_name
	);

	Texture* initLoadTexture(std::string_view filename);

	ShaderProgram initLoadShaderProgram(
		std::string_view vertex_name,
		std::string_view fragment_name,
		std::string_view geometry_name
	); // TODO:

	bool readFile(const std::filesystem::path& path, std::string& content);
};