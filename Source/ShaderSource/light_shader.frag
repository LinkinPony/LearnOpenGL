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

uniform int index;

uniform Light light[MAX_LIGHT_NUM];

void main()
{
    vec3 color = (light[index].diffuse + light[index].specular +  light[index].ambient) / 3;
    FragColor = vec4(color,0.5);
}