//---------------------------------------------+
// ���}�b�s���O (�}���`�����_�[�Ή�)
//---------------------------------------------+
#version 420
// attribute
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

// uniform�o�b�t�@�u���b�N (�s��)
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};

// �o��
// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec3 fragNormal;                // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;              // ���[���h�X�y�[�X��̍��W
	// ���}�b�v�p
	vec3 fragEnvNormal;
	vec3 fragEnvWorldPos;
}vs_out;

// ����
uniform mat4 u_worldTransform;
uniform mat4 u_offset;

void main()
{
	vec4 pos = u_worldTransform * vec4(a_pos, 1.0);
	gl_Position = u_projection * u_view * pos;

	vs_out.fragNormal = mat3(transpose(u_worldTransform)) * a_normal;
	vs_out.fragWorldPos = pos.xyz;                                                 // ���[���h��̈ʒu�x�N�g�����o��

	// ���}�b�v�p�m�[�}��+���[���h���W
	vs_out.fragEnvNormal = vs_out.fragNormal;
	vs_out.fragEnvNormal = mat3(u_offset) * vs_out.fragEnvNormal;
	vs_out.fragEnvWorldPos = pos.xyz;
}