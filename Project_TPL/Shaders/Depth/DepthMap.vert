#version 420
layout (location = 0) in vec3 a_pos;

uniform mat4 u_lightSpaceMatrix;    // ライトスペース変換行列
uniform mat4 u_worldTransform;

void main()
{

    gl_Position = u_lightSpaceMatrix * u_worldTransform * vec4(a_pos, 1.0);
}  