//-----------------------------------------------------+
// ���}�b�s���O (�}���`�����_�[�Ή�)
//-----------------------------------------------------+
#version 420
// �e�o�b�t�@�ւ̏o�� (GBuffer)
layout (location = 0) out vec3 out_gPos;
layout (location = 1) out vec3 out_gNormal;
layout (location = 2) out vec4 out_gAlbedoSpec;
layout (location = 3) out vec4 out_gBrightColor;

// ���_�V�F�[�_����̓���
in VS_OUT
{
	vec3 fragNormal;
	vec3 fragWorldPos;
	// ���}�b�v�p
	vec3 fragEnvNormal;
	vec3 fragEnvWorldPos;
}fs_in;

//----------------------------------------------------+
// uniform buffer block
// camera variables
layout(std140, binding = 1) uniform CameraVariable
{
	vec3 u_viewPos;
};
// triggers
layout(std140, binding = 2) uniform Triggers
{
	int u_enableBloom;
};

uniform samplerCube u_skybox;        // �L���[�u�}�b�v(�X�J�C�{�b�N�X)

uniform float u_luminance = 1.0f;
uniform float u_alpha;

void main()
{
	float ratio = 1.00 / 1.52;      // ���˗�
	vec3 I = normalize(fs_in.fragEnvWorldPos - u_viewPos);        // �J�����̌����x�N�g��
	vec3 R = refract(I, normalize(fs_in.fragEnvNormal), ratio);       // �J�����̌����x�N�g���Ɩ@�����甽�˃x�N�g���𐶐�

	// �o�̓J���[
	vec4 resultColor = vec4(texture(u_skybox, R).rgb, u_alpha);

	// GBuffer�o��
	out_gPos = fs_in.fragWorldPos;
	out_gNormal = fs_in.fragNormal;
	out_gAlbedoSpec.rgb = resultColor.rgb;
	out_gAlbedoSpec.a = u_alpha;

	if(u_enableBloom == 1)
	{
		out_gBrightColor = vec4(resultColor.rgb, u_alpha) * u_luminance;
	}

}