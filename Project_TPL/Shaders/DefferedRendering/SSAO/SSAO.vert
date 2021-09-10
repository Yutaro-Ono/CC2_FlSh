//----------------------------------------------------------------------------------+
// @file        SSAO.vert
// @brief       SSAO Main
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_vertexPos;
layout(location = 1) in vec2 a_texCoords;

// out structure (Output to Fragment)
out VS_OUT
{
	vec2 fragTexCoords;
}vs_out;


void main()
{
	gl_Position = vec4(a_vertexPos, 1.0);
	vs_out.fragTexCoords = a_texCoords;
}