#version 330 core
layout (location = 0) in vec3 aPos;  // 位置变量的属性位置值为0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;

out vec2 TexCoord;
void main()
{
    gl_Position = m_projection * m_view * m_model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}