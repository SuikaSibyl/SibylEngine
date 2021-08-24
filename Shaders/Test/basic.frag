#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    FragColor = vec4(texture(u_Texture, v_TexCoord).rgb, 1);
}