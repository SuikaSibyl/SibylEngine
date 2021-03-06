//////////////////////////////////////////////////////////////////////
/////                       Vertex Shader                       //////
//////////////////////////////////////////////////////////////////////
#type VS
#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec2 aUV; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色
out vec2 v_TexCoord; // 向片段着色器输出一个颜色

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

void main()
{
    gl_Position = Model * Projection* View * vec4(aPos, 1.0);
    ourColor = aPos; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
    v_TexCoord = aUV;
}

//////////////////////////////////////////////////////////////////////
/////                     Fragment Shader                       //////
//////////////////////////////////////////////////////////////////////
#type FS
#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 v_TexCoord;

uniform vec4 Color;
uniform sampler2D u_Texture;

void main()
{
    FragColor = Color*texture(u_Texture, v_TexCoord);
}