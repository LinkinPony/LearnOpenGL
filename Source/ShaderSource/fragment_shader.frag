#version 330 core

#define MAX_LIGHT_NUM 16

struct Light{
    int type;//0-point light 1-parallel light
    float intensity;
    vec3 position;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_specular_0;
uniform vec3 light_color;

uniform Light light[MAX_LIGHT_NUM];

void main()
{

    FragColor = texture(texture_diffuse_0,TexCoord);
}