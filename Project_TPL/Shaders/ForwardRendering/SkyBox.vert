//-------------------------------------------------------+
// �X�J�C�{�b�N�X�p���_�V�F�[�_
//-------------------------------------------------------+
#version 330 core
// attribute
layout (location = 0) in vec3 a_pos;

out vec3 TexCoords;

uniform mat4 u_projection;
uniform mat4 u_invTransView;
uniform mat4 u_offset;

void main()
{
	// �e�N�X�`�����W�Ƀ��[�J���ʒu�x�N�g����ݒ�
	TexCoords = a_pos;

	vec4 pos = vec4(a_pos, 1.0) * u_offset * u_invTransView * u_projection;

	gl_Position = pos.xyww;     // z������w�Œu��������(�ŉ��\��)

}