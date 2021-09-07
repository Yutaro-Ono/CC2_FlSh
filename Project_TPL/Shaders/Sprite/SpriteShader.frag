//-------------------------------------------------------+
// �X�v���C�g(2D)�p�t���O�����g�V�F�[�_
//-------------------------------------------------------+
#version 420
// �o�̓J���[
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;

// ���_�V�F�[�_����̓���
in vec2 FragTexCoords;           // �e�N�X�`�����W

// triggers
layout(std140, binding = 2) uniform Triggers
{
	int u_enableBloom;
};

// uniform
uniform sampler2D u_texture;     // �e�N�X�`���T���v�����O
uniform float u_intensity = 1.0f;  // �P�x����

void main()
{

	// �e�N�X�`������J���[���T���v�����O
	out_color = texture(u_texture, FragTexCoords);
	out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
	vec3 result = out_color.rgb;
	float brightness = dot(result, vec3(0.0, 0.0, 1.0)) * u_intensity;

	if(u_enableBloom == 1)
	{
		if(brightness > 0.5)                                              // �P�x��0.5�ȏ�ō��P�x�o�b�t�@�֏o��
	    {
		    out_brightColor = out_color * 0.5f;
	    }
	}

}