#version 460 core

out vec4 frag_color;

in vec3 frag_norm;
in vec2 frag_uv;
in vec3 frag_world_pos;

vec3 light_color = vec3(1.0f);
vec3 light_pos = vec3(0.0f);

uniform vec3 camera_pos;

uniform sampler2D tex;

void main()
{
	float ambient = 0.1f;

	vec3 to_light_dir = normalize(light_pos - frag_world_pos);
	vec3 to_camera_dir = normalize(camera_pos - frag_world_pos);
	vec3 refl = reflect(-to_light_dir,frag_norm);

	float diff = max(dot(frag_norm, to_light_dir), 0.0f);

	float spec_inter = max(dot(to_camera_dir,refl), 0.0f);
	
	float spec = pow(spec_inter, 64);
	
	vec3 tex_color = vec3(texture(tex, frag_uv));
	
	frag_color = vec4(((ambient + diff + spec) * light_color) * tex_color, 1.0f);
}