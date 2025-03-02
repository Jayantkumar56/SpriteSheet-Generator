

#type Vertex
#version 450 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoord;

layout(location = 0) out vec3 o_Position;
layout(location = 1) out vec3 o_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
	o_Normal    = v_Normal;
    o_Position  = v_Position;
	gl_Position = u_ViewProjection * u_Transform * vec4(v_Position, 1.0f);
}







#type Fragment
#version 450 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normal;

layout(location = 0) out vec4 o_Color;

vec4 DirectionalLight(vec4 fragColor) {
	vec3 lightColor     = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightDirection = normalize(-vec3(-0.2f, -1.0f, -0.3f));

	float ambientStrength  = 0.2f;
	float diffuseStrength  = max(dot(v_Normal, lightDirection), 0.0f);
//	float specularStrength = 0.5;

//	vec3 viewDir    = normalize(u_CameraPosition - v_Position);
//	vec3 reflectDir = reflect(-lightDirection, v_Normal);
//	float spec      = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 ambient  = ambientStrength  * lightColor;
	vec3 diffuse  = diffuseStrength  * lightColor;
//    vec3 specular = specularStrength * spec * lightColor;

	return fragColor * vec4(ambient.xyz + diffuse.xyz, 1.0f);
}


void main() {
	o_Color = DirectionalLight(vec4(1.0f, 0.5f, 0.31f, 1.0f));
}




