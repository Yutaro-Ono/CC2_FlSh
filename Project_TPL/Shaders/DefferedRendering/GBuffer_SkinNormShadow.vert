//----------------------------------------------------+
// �X�L�����胁�b�V���p���_�V�F�[�_
//----------------------------------------------------+
#version 420
// Attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in uvec4 a_skinBones;
layout(location = 3) in vec4 a_skinWeights;
layout(location = 4) in vec2 a_texCoords;
layout(location = 5) in vec3 a_tangent;

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
//
// �ϊ��s�� (���[���h�A�r���[�A�v���W�F�N�V����)
uniform mat4 u_worldTransform;
// �s��p���b�g
uniform mat4 u_matrixPalette[196];
// ���C�g��ԍs��
uniform mat4 u_lightSpaceMatrix;
// �������W�E�J�������W
uniform vec3 u_lightPos;


// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec2 fragTexCoords;          // �e�N�X�`�����W
	vec3 fragNormal;            // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;          // ���[���h�X�y�[�X��̍��W
	vec3 fragViewPos;           // �r���[���W
	vec4 fragPosLightSpace;     // ���C�g�X�y�[�X��̍��W
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;

}vs_out;

void main()
{
	// ���_���W���l�����ɕϊ�
	vec4 pos = vec4(a_pos, 1.0);
	
	// �e�N�X�`�����W���o��
	vs_out.fragTexCoords = a_texCoords;

	// �X�L���ʒu���Z�o
	vec4 skinnedPos = a_skinWeights.x * (pos * u_matrixPalette[a_skinBones.x]);
	skinnedPos += a_skinWeights.y * (pos * u_matrixPalette[a_skinBones.y]);
	skinnedPos += a_skinWeights.z * (pos * u_matrixPalette[a_skinBones.z]);
	skinnedPos += a_skinWeights.w * (pos * u_matrixPalette[a_skinBones.w]);
	// ���[���h��̃X�L���ʒu��ۑ�
	vs_out.fragWorldPos = (u_worldTransform * skinnedPos).xyz;
	// �N���b�v��Ԃ֒��_��ϊ����o�͂ɓn��
	gl_Position = u_projection * u_view * u_worldTransform * skinnedPos;


	// �X�L���ʒu�̖@�����Z�o
	vec4 skinnedNormal = vec4(a_normal, 0.0f);
	skinnedNormal = (u_matrixPalette[a_skinBones.x] * skinnedNormal) * a_skinWeights.x
		+ (u_matrixPalette[a_skinBones.y] * skinnedNormal) * a_skinWeights.y
		+ (u_matrixPalette[a_skinBones.z] * skinnedNormal) * a_skinWeights.z
		+ (u_matrixPalette[a_skinBones.w] * skinnedNormal) * a_skinWeights.w;
	// �@�������[���h��ԏ�ɕϊ�(w = 0)
	vs_out.fragNormal = mat3(transpose(inverse(u_worldTransform))) * a_normal;

	vs_out.fragPosLightSpace = vec4(vs_out.fragWorldPos, 1.0) * u_lightSpaceMatrix;

	//------------------------------------------------------------------------------------+
	// Tangent�v�Z
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
	vs_out.TangentFragPos = TBN * vec3(u_worldTransform * vec4(skinnedPos.xyz, 0.0f));      // �ڋ�Ԃɂ����钸�_���W
}

