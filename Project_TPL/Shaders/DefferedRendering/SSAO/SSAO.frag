//----------------------------------------------------------------------------------+
// @file        SSAO.frag
// @brief       SSAO Main
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output color
out float FragColor;
//----------------------------------------------------+
// uniform buffer block
// matrices
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};

// input structure from vertex shader
in VS_OUT
{
	vec2 fragTexCoords;
}fs_in;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_texNoise;

uniform vec3 u_samples[64];

// �e��p�����[�^
uniform int u_kernelSize = 64;
uniform float u_radius = 0.5f;
float u_bias = 0.025f;
uniform float u_screenW;
uniform float u_screenH;

// ��ʂ̏c���T�C�Y���m�C�Y�̑傫���Ŋ���A����Ɋ�Â��ăm�C�Y�e�N�X�`������ʏ�Ƀ^�C��������
const vec2 noiseScale = vec2(u_screenW / 4.0f, u_screenH / 4.0f);


void main()
{
	// �eSSAO�̓��͏����擾
	vec3 fragPos = texture(u_gPosition, fs_in.fragTexCoords).xyz;
	vec3 normal = normalize(texture(u_gNormal, fs_in.fragTexCoords).rgb);
    vec3 randomVec = normalize(texture(u_texNoise, fs_in.fragTexCoords * noiseScale).xyz);
    // TBN�s��̍쐬:�^���W�F���g��ԁ��r���[���
    //vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 tangent = normalize(randomVec - dot(randomVec, normal) * normal);

    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = transpose(mat3(tangent, bitangent, normal));


    // �T���v���J�[�l���𔽕����A�I�N���[�W�������Z�o
    float occlusion = 0.0;
    for(int i = 0; i < u_kernelSize; ++i)
    {
        // �T���v�����O���W���擾
        vec3 samplePos = u_samples[i] * TBN;                     // �^���W�F���g����r���[�X�y�[�X��
        samplePos = fragPos + samplePos * u_radius; 
        
        // ���W�̃T���v�����O (�e�N�X�`���T���v�����O) (�X�N���[���^�e�N�X�`����̈ʒu���擾����)
        vec4 offset = vec4(samplePos, 1.0);
        offset = u_projection * offset;                        // �r���[��Ԃ���N���b�v��Ԃ�
        offset.xyz /= offset.w;                                // �p�[�X�y�N�e�B�u�̏���
        offset.xyz = offset.xyz * 0.5 + 0.5;                   // �����W���ɕϊ� �� 0.0 - 1.0
        
        // �[�x�̎擾
        float sampleDepth = texture(u_gPosition, offset.xy).z; // �J�[�l���T���v���̐[�x�l���擾
        
        // �����W�̃`�F�b�N�ƒ~��
        float rangeCheck = smoothstep(0.0, 1.0, u_radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + u_bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / u_kernelSize);
    
    FragColor = occlusion;

}