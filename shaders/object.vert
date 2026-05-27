#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal;

out vec3 frag_norm;
out vec2 frag_uv;
out vec3 frag_cam_pos;

void main()
{
	vec4 cam = view * model * vec4(pos, 1.0);
	gl_Position = proj * cam;

	frag_norm = normal * norm;
	frag_uv = uv;
	frag_cam_pos = vec3(cam);
}