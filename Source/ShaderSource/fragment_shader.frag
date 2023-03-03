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
in vec3 world_pos;
in vec3 normal;
out vec4 FragColor;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_specular_0;
uniform vec3 light_color;
uniform vec3 eye_direction;
uniform int light_num;
uniform Light light[MAX_LIGHT_NUM];

void main()
{
    vec3 light_result = vec3(0,0,0);
    for(int i = 0;i < light_num;i++){
        vec3 light_direction = normalize(light[i].position - world_pos);
        //vec3 cur_normal = vec3(texture(texture_specular_0,TexCoord));
        float r = 1;
        if(light[i].type != 1){
            r = distance(world_pos,light[i].position);
        }
        if(light[i].type == 1){
            light_direction = normalize(light[i].direction);
        }
        
        vec3 diffuse = light[i].diffuse * light[i].intensity / (r * r) * max(0.0,dot(normal,light_direction));
        vec3 bisector = normalize(light_direction + eye_direction);
        vec3 specular = light[i].specular * light[i].intensity / (r * r) * pow(max(0.0,dot(normal,bisector)),50);
        vec3 ambient = light[i].ambient * 0.3;
        light_result += diffuse + specular + ambient;
    }
    FragColor = vec4(light_result,1.0) * texture(texture_diffuse_0,TexCoord);
}