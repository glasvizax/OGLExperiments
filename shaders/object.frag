#version 460 core

out vec4 frag_color;

in vec3 frag_norm;
in vec2 frag_uv;
in vec3 frag_cam_pos;

struct Material
{
	sampler2D diffuse_map;
	sampler2D specular_map;

	float shininess;
}; 

uniform Material material;

struct FragmentMaterial
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

struct Light 
{
	vec3 cam_position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

vec3 getAmbient(FragmentMaterial frag_mat)
{
	return frag_mat.ambient * light.ambient;
}

vec3 getDiffuse(vec3 normal, vec3 to_light_dir, FragmentMaterial frag_mat)
{
	float diff = max(dot(normal, to_light_dir), 0.0f);
	vec3 diffuse = frag_mat.diffuse * diff * light.diffuse;
	return diffuse;
}

vec3 getSpecular(vec3 normal, vec3 to_light_dir, vec3 to_camera_dir, FragmentMaterial frag_mat)
{
	float ndotl = dot(normal, to_light_dir);

    if (ndotl <= 0.0)
    {
		return vec3(0.0);
	}

	vec3 refl = reflect(-to_light_dir, normal);
	float spec_inter = max(dot(to_camera_dir, refl), 0.0);
	float spec = pow(spec_inter, material.shininess);
	vec3 specular = light.specular * spec * frag_mat.specular;
	return specular;
}

void main()
{
	vec3 frag_diffuse = vec3(texture(material.diffuse_map, frag_uv));
	vec3 frag_specular = vec3(texture(material.specular_map, frag_uv));
	
	FragmentMaterial frag_mat;
	frag_mat.ambient = frag_diffuse;
	frag_mat.diffuse = frag_diffuse;
	frag_mat.specular = frag_specular;
	frag_mat.shininess = material.shininess;

	vec3 norm = normalize(frag_norm);

	vec3 to_light_dir = normalize(light.cam_position - frag_cam_pos);
	vec3 to_camera_dir = normalize(-frag_cam_pos);
	
	vec3 ambient = getAmbient(frag_mat);
	vec3 diffuse = getDiffuse(norm, to_light_dir, frag_mat);
	vec3 specular = getSpecular(norm, to_light_dir, to_camera_dir, frag_mat);

	frag_color = vec4((ambient + diffuse + specular), 1.0f);
}