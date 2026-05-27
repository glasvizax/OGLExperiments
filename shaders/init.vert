#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 frag_norm;
out vec2 frag_uv;
out vec3 frag_world_pos;

void main()
{
	vec4 world = model * vec4(pos, 1.0);
	gl_Position = proj * view * world;

	frag_norm = norm;
	frag_uv = uv;
	frag_world_pos = vec3(world);
}