#version 420
layout (location = 0) in vec3 a_pos;

uniform mat4 u_lightSpaceMatrix;    // ���C�g�X�y�[�X�ϊ��s��
uniform mat4 u_worldTransform;

void main()
{

    gl_Position = u_lightSpaceMatrix * u_worldTransform * vec4(a_pos, 1.0);
}  