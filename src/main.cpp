#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Aliases.h"
#include "Camera.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

Vertex g_cube_vertices[] = {
	// -Z (front)
	{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
	{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
	{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
	{{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

	// +Z (back)
	{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
	{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
	{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},

	// -X (left)
	{{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

	// +X (right)
	{{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

	// -Y (bottom)
	{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
	{{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
	{{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
	{{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},

	// +Y (top)
	{{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
	{{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
	{{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
};

glm::uvec3 g_cube_indices[] = {
	{0, 1, 2},
	{0, 2, 3},
	{4, 5, 6},
	{4, 6, 7},
	{8, 9, 10},
	{8, 10, 11},
	{12, 13, 14},
	{12, 14, 15},
	{16, 17, 18},
	{16, 18, 19},
	{20, 21, 22},
	{20, 22, 23}
};

void keyCallback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mods
);

void glfwErrorCallback(int error_code, const char* description);

class Engine
{
	GLFWwindow* m_window = nullptr;

	ResourceManager m_resource_manager;
	Camera m_camera;

	friend void keyCallback(
		GLFWwindow* window,
		int key,
		int scancode,
		int action,
		int mods
	);

public:
	ResourceManager& getResourceManager()
	{
		return m_resource_manager;
	}

	GLFWwindow* getWindowGLFW()
	{
		return m_window;
	}

	Camera& getCamera()
	{
		return m_camera;
	}

	bool init()
	{
		if (glfwInit() == GLFW_FALSE)
		{
			std::cerr << "could init glfw" << std::endl;
			return false;
		}

		glfwSetErrorCallback(glfwErrorCallback);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow( //
			1200,
			800,
			"OGLE Window",
			nullptr,
			nullptr
		);

		if (!m_window)
		{
			std::cerr << "could init glfw window" << std::endl;
			return false;
		}

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
		{
			std::cerr << "failed to initialize glad" << std::endl;
			return false;
		}

		auto last_time = chrono::steady_clock::now();

		glfwSetKeyCallback(m_window, keyCallback);

		int width, height;
		glfwGetFramebufferSize(m_window, &width, &height);
		float aspect_ratio = static_cast<float>(width) /
							 static_cast<float>(height);

		m_camera.setAspectRatio(aspect_ratio);
		return true;

		m_resource_manager.init();
	}

	int mainLoop()
	{
		auto last_time = chrono::steady_clock::now();
		float time_integral = 0.0f;

		glViewport(0, 0, 1200, 800);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);

		Mesh cube_mesh = Mesh(
			std::span<Vertex>(g_cube_vertices),
			std::span<glm::uvec3>(g_cube_indices)
		);

		ShaderProgram light_sp = m_resource_manager.initLoadShaderProgram(
			"light.vert",
			"light.frag"
		);

		ShaderProgram object_sp = m_resource_manager.initLoadShaderProgram(
			"object.vert",
			"object.frag"
		);

		// Texture* af_tex =
		// m_resource_manager.initLoadTexture("awesomeface.png");

		glm::vec3 light_pos(-7.0f, -2.0f, 4.0f);
		glm::vec3 light_scale(0.3f);
		glm::mat4 light_model(1.0f);

		light_model = glm::translate(light_model, light_pos);
		light_model = glm::scale(light_model, light_scale);

		glm::vec3 object_pos(0.0f, 0.0f, -7.0f);
		glm::mat4 object_model(1.0f);
		object_model = glm::translate(object_model, object_pos);

		Material material;
		material.ambient = glm::vec3(0.1f);
		material.diffuse = glm::vec3(0.75f, 0.2f, 0.2f);
		material.specular = glm::vec3(0.5f);
		material.shininess = 64;

		Light light;
		light.ambient = glm::vec3(0.2f);
		light.diffuse = glm::vec3(1.0f);
		light.specular = glm::vec3(0.4f);

		while (!glfwWindowShouldClose(m_window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto time = chrono::steady_clock::now();
			float delta = chrono::duration<float>(time - last_time).count();
			last_time = time;
			time_integral += delta;

			m_camera.update(delta);

			glm::mat4 proj = m_camera.getPerspectiveMatrix();
			glm::mat4 view = m_camera.getViewMatrix();

			glm::mat3 object_normal = glm::mat3(
				glm::inverse(glm::transpose(view * object_model))
			);

			glm::vec3 light_cam_pos = glm::vec3(
				view * glm::vec4(light_pos, 1.0f)
			);

			light.cam_position = light_cam_pos;

			object_sp.bind();
			object_sp.setMat("proj"_id, proj);
			object_sp.setMat("view"_id, view);
			object_sp.setMat("model"_id, object_model);
			object_sp.setMat("normal"_id, object_normal);
			setMaterial("material", material, object_sp);
			setLight("light", light, object_sp);

			cube_mesh.draw();

			light_sp.bind();
			light_sp.setMat("proj"_id, proj);
			light_sp.setMat("view"_id, view);
			light_sp.setMat("model"_id, light_model);
			light_sp.setVec("light_color"_id, light.diffuse);
			cube_mesh.draw();

			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}

		return EXIT_SUCCESS;
	}

	void destroy()
	{
		glfwTerminate();
	}

	~Engine()
	{
		destroy();
	}
};

Engine g_engine;

int main()
{
	if (g_engine.init())
	{
		return g_engine.mainLoop();
	}
}

void keyCallback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mods
)
{
	switch (key)
	{
		case (GLFW_KEY_O):
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		}

		case (GLFW_KEY_W):
		{
			g_engine.m_camera.moveForward();
			break;
		}

		case (GLFW_KEY_S):
		{
			g_engine.m_camera.moveBackward();
			break;
		}
		case (GLFW_KEY_D):
		{
			g_engine.m_camera.moveRight();
			break;
		}

		case (GLFW_KEY_A):
		{
			g_engine.m_camera.moveLeft();
			break;
		}

		case (GLFW_KEY_Q):
		{
			g_engine.m_camera.turnLeft();
			break;
		}

		case (GLFW_KEY_E):
		{
			g_engine.m_camera.turnRight();
			break;
		}

		case (GLFW_KEY_Z):
		{
			g_engine.m_camera.lookDown();
			break;
		}

		case (GLFW_KEY_X):
		{
			g_engine.m_camera.lookUp();
			break;
		}
	}
}

void glfwErrorCallback(int error_code, const char* description)
{
	std::cerr << "error code: " << error_code << "description: " << description
			  << std::endl;
}
