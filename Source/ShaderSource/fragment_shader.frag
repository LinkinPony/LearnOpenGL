#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_specular_0;
uniform vec3 light_color;

void main()
{

    FragColor = texture(texture_diffuse_0,TexCoord);
}