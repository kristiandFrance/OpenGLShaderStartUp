#version 460 core

#define MAX_POINT_LIGHTS 4

struct PointLight
{
    vec3 Position;
    vec3 Color;
    float SpecularStrength;
};

// Inputs from vertex shader
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

// Uniform inputs
uniform sampler2D Texture0;
uniform samplerCube Texture_Skybox;
uniform vec3 CameraPos;
uniform float AmbientStrength = 0.15f;
uniform vec3 AmbientColor = vec3(1.0f, 1.0f, 1.0f);
uniform float ObjectShininess = 32.0f;

uniform PointLight PointLightArray[MAX_POINT_LIGHTS];
uniform unsigned int PointLightCount;

out vec4 FinalColor;

vec3 CalculateLight_Point(PointLight light, vec3 normal, vec3 fragPos)
{
    vec3 lightDir = normalize(light.Position - fragPos);

    // Diffuse component
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.Color;

    // Specular component
    vec3 viewDir = normalize(CameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), ObjectShininess);
    vec3 specular = light.SpecularStrength * spec * light.Color;

    return diffuse + specular;
}

void main()
{
    vec3 ambient = AmbientStrength * AmbientColor;
    vec3 totalLight = vec3(0.0);

    for (unsigned int i = 0; i < PointLightCount; ++i)
    {
        totalLight += CalculateLight_Point(PointLightArray[i], normalize(FragNormal), FragPos);
    }

    totalLight += ambient;

    vec3 viewDir = normalize(CameraPos - FragPos);
    vec3 reflectDir = reflect(-viewDir, normalize(FragNormal));

    vec4 objectColor = texture(Texture0, FragTexCoords);
    vec4 reflectionColor = texture(Texture_Skybox, reflectDir);

    FinalColor = mix(objectColor, reflectionColor, 0.5) * vec4(totalLight, 1.0);
}
