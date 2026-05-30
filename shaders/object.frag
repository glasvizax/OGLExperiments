#version 460 core

out vec4 frag_color;

in vec3 frag_norm;
in vec2 frag_uv;
in vec3 frag_cam_pos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
}; 

uniform Material material;

struct Light 
{
	vec3 cam_position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

vec3 getAmbient()
{
	return material.ambient * light.ambient;
}

vec3 getDiffuse(vec3 normal, vec3 to_light_dir)
{
	float diff = max(dot(normal, to_light_dir), 0.0f);
	vec3 diffuse = material.diffuse * diff * light.diffuse;
	return diffuse;
}

vec3 getSpecular(vec3 normal, vec3 to_light_dir, vec3 to_camera_dir)
{
	float ndotl = dot(normal, to_light_dir);

    if (ndotl <= 0.0)
    {
		return vec3(0.0);
	}

	vec3 refl = reflect(-to_light_dir, normal);
	float spec_inter = max(dot(to_camera_dir, refl), 0.0);
	float spec = pow(spec_inter, material.shininess);
	vec3 specular = light.specular * spec * material.specular;
	return specular;
}

void main()
{
	vec3 norm = normalize(frag_norm);

	vec3 to_light_dir = normalize(light.cam_position - frag_cam_pos);
	vec3 to_camera_dir = normalize(-frag_cam_pos);
	
	vec3 ambient = getAmbient();
	vec3 diffuse = getDiffuse(norm, to_light_dir);
	vec3 specular = getSpecular(norm, to_light_dir, to_camera_dir);

	frag_color = vec4((ambient + diffuse + specular), 1.0f);
}