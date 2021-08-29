//-------------------------------------------------------------------------+
// GBuffer�p ���b�V�����_�V�F�[�_
//-------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_tangent;

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

// �������W�E�J�������W
uniform vec3 u_lightPos;

// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec2 fragTexCoords;              // �e�N�X�`�����W
	vec3 fragNormal;                // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;              // ���[���h�X�y�[�X��̍��W
	vec3 fragViewPos;               // �J�������W
	vec4 fragPosLightSpace;         // ���C�g�X�y�[�X��̍��W
	// �^���W�F���g��ԓ��̊e���W
	vec3 TangentLightPos;           // ���C�g(������)�̍��W
	vec3 TangentViewPos;            // �r���[(�J����)�̍��W
	vec3 TangentFragPos;            // �t���O�����g���W

}vs_out;


void main()
{
	vec4 pos = u_worldTransform * vec4(a_pos, 1.0);
	gl_Position = u_projection * u_view * pos;
	vs_out.fragTexCoords   = a_texCoords;

	vs_out.fragWorldPos     = pos.xyz;
	vs_out.fragNormal      =  mat3(transpose(inverse(u_worldTransform))) * a_normal;
	vs_out.fragViewPos = u_viewPos;
	// ���[���h���W�̒��_�����C�g�X�y�[�X�ɕϊ����ĕۑ�
	vs_out.fragPosLightSpace = vec4(vs_out.fragWorldPos, 1.0) * u_lightSpaceMatrix;

	vec3 T, B, N;     // TBN�s��
	N = a_normal;     // �@������
	T = a_tangent;    // �ڋ��(TangentSpace)����

	// �@���ɑ΂���ڋ�Ԃ̍Ē��s��
	T = normalize(T - dot(T, N) * N);
	// �ڋ�ԃx�N�g���Ɩ@���x�N�g���̊O�ς��琂���x�N�g��B(BiTangent)���擾
	B = cross(N, T);

	T = normalize(vec3(u_worldTransform * vec4(T, 0.0f)));
	B = normalize(vec3(u_worldTransform * vec4(B, 0.0f)));
	N = normalize(vec3(u_worldTransform * vec4(N, 0.0f)));

	// TBN�s����t�s��Ƃ��Đ���
	mat3 TBN = transpose(mat3(T, B, N));

	// �ڋ�ԓ��̍��W��`
	vs_out.TangentLightPos = TBN * u_lightPos;                                     // �ڋ�Ԃɂ���������ʒu
	vs_out.TangentViewPos = TBN * u_viewPos;                                       // �ڋ�Ԃɂ�����r���[���W
	vs_out.TangentFragPos = TBN * vec3(u_worldTransform * vec4(a_pos, 0.0f));      // �ڋ�Ԃɂ����钸�_���W

}