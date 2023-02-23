#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 light_color;

void main()
{
    FragColor = vec4(light_color,1.0) * texture(texture1,TexCoord);
}