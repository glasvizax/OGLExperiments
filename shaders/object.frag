#version 460 core

out vec4 frag_color;

in vec3 frag_norm;
in vec2 frag_uv;
in vec3 frag_cam_pos;

uniform vec3 light_cam_pos;
uniform vec3 light_color;
uniform vec3 object_color;

void main()
{
	float amb = 0.1f;
	vec3 ambient = amb * light_color;
	//frag_norm = normalize(frag_norm);
	vec3 _frag_norm =  normalize(frag_norm);

	vec3 to_light_dir = normalize(light_cam_pos - frag_cam_pos);
	vec3 to_camera_dir = normalize(-frag_cam_pos);
	
	vec3 refl = reflect(-to_light_dir, _frag_norm);

	float diff = max(dot(_frag_norm, to_light_dir), 0.0f);
	vec3 diffuse = diff * light_color;

	float spec_inter = max(dot(to_camera_dir,refl), 0.0f);
	float spec = pow(spec_inter, 256.0f);
	vec3 specular = spec * light_color;

	frag_color = vec4((ambient + diffuse + specular) * object_color, 1.0f);
}