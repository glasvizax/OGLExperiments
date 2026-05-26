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

struct Engine
{
	GLFWwindow* window;
	Camera m_camera;
};

Engine g_engine;

int main()
{

	if (glfwInit() == GLFW_FALSE)
	{
		std::cerr << "could init glfw" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	g_engine.window = glfwCreateWindow( //
		1200,
		800,
		"OGLE Window",
		nullptr,
		nullptr
	);

	if (!g_engine.window)
	{
		std::cerr << "could init glfw window" << std::endl;
	}

	glfwMakeContextCurrent(g_engine.window);

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout << "failed to initialize glad" << std::endl;
	}

	auto last_time = chrono::steady_clock::now();

	glfwSetKeyCallback(g_engine.window, keyCallback);

	glViewport(0, 0, 1200, 800);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	float time_integral = 0.0f;

	Mesh cube_mesh = Mesh(
		std::span<Vertex>(g_cube_vertices),
		std::span<glm::uvec3>(g_cube_indices)
	);

	glm::mat4 cube_model(1.0f);
	glm::translate(cube_model, glm::vec3(0.0f, 0.0f, -7.0f));

	ShaderProgram init_program = initLoadShaderProgram(
		"init.vert",
		"init.frag"
	);

	init_program.bind();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(g_engine.window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto time = chrono::steady_clock::now();
		float delta = chrono::duration<float>(time - last_time).count();
		last_time = time;

		time_integral += delta;

		g_engine.m_camera.update(delta);

		glm::mat4 proj = g_engine.m_camera.getPerspectiveMatrix();
		glm::mat4 view = g_engine.m_camera.getViewMatrix();

		init_program.setMat("proj"_id, proj);
		init_program.setMat("view"_id, view);
		init_program.setMat("model"_id, cube_model);

		cube_mesh.draw();

		glfwSwapBuffers(g_engine.window);
		glfwPollEvents();
	}

	return 0;
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
	}

	/*
	else if (last_char == L'q' || last_char == L'é')
	{
		m_camera.turnLeft();
	}
	else if (last_char == L'e' || last_char == L'ó')
	{
		m_camera.turnRight();
	}
	else if (last_char == L'z' || last_char == L'ÿ')
	{
		m_camera.lookDown();
	}
	else if (last_char == L'x' || last_char == L'÷')
	{
		m_camera.lookUp();
	}*/
}
