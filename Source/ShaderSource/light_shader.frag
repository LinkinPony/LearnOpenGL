#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_diffuse_0;
uniform vec3 light_color;

void main()
{

    float ka = 0.3;
    FragColor = texture(texture_diffuse_0,TexCoord);
}