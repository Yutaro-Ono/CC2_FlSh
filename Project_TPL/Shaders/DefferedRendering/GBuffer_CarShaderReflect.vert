//-------------------------------------------------------------------------+
// GBuffer�E�Ԃ̃��f���p ���_�V�F�[�_
// ���}�b�v�E���ˌv�Z
//-------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

// uniform�o�b�t�@�u���b�N
// 0.�s��
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};
// 1.�J�������W
layout(std140, binding = 1) uniform CameraVariable
{
	vec3 u_viewPos;
};

// ���[���h�ϊ��s��E���C�g�X�y�[�X
uniform mat4 u_worldTransform;
uniform mat4 u_lightSpaceMatrix;   
// �������W
uniform vec3 u_lightPos;
// ���˃}�b�v�I�t�Z�b�g(�f�荞�݂̊p�x�����p)
uniform mat4 u_offset;

// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec2 fragTexCoords;              // �e�N�X�`�����W
	vec3 fragNormal;                // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;              // ���[���h�X�y�[�X��̍��W
	vec3 fragViewPos;               // �J�������W
	vec4 fragPosLightSpace;         // ���C�g�X�y�[�X��̍��W
	// ���}�b�v�p
	vec3 fragEnvNormal;
	vec3 fragEnvWorldPos;
}vs_out;


void main()
{
	vec4 pos =  u_worldTransform * vec4(a_pos, 1.0);
	gl_Position = u_projection * u_view * pos;

	vs_out.fragTexCoords   = a_texCoords;
	vs_out.fragViewPos     = u_viewPos;
	// ���[���h���W�̒��_�����C�g�X�y�[�X�ɕϊ����ĕۑ�
	vs_out.fragPosLightSpace = u_lightSpaceMatrix * vec4(vs_out.fragWorldPos, 1.0);

	//vs_out.fragNormal = vs_out.fragNormal;
	vs_out.fragNormal = mat3(transpose(inverse(u_worldTransform))) * a_normal;
	vs_out.fragWorldPos = pos.xyz;                  // ���[���h��̈ʒu�x�N�g�����o��

	// ���}�b�v�p�m�[�}��+���[���h���W
	vs_out.fragEnvNormal = vs_out.fragNormal;
	vs_out.fragEnvNormal = mat3(u_offset) * vs_out.fragEnvNormal;
	vs_out.fragEnvWorldPos = pos.xyz;

}