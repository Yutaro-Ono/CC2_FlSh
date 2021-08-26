//-----------------------------------------------------------------------+
// ���_�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once


class VertexArray
{
public:

	//-------------------------------------------------------------------------------------------------------------------+
    // ���_���C�A�E�g�̃f�[�^�T�C�Y
    // POS_NORMAL_UV = 8 * sizeof(float) = 32 bytes
    // | POSITION  | NORMAL    | TEXTURE |
    // | x | y | z | x | y | z | u  | v  | 
    //
    // ���^���W�F���g��Ԃ͒ʏ�̍��W�Ɠ��� <3 * sizeof(float) = 12byte>
    //
    // POS_NORMAL_SKIN_UV
    // POS_NORMAL_SKIN_UV = (8 * sizeof(float)) + (8 * sizeof(char)) = 40 bytes
    // | POSITION  | NORMAL    | BONES   | WEIGHT  |TEXTURE|
    // | x | y | z | x | y | z | char[4] | char[4] | u | v |
    //                                    ��WEIGHT�̊m�ۂ�char�����A���x���K�v�Ȃ��̂�8bit�Œ菬���Ƃ��Ďg�p����
    //-------------------------------------------------------------------------------------------------------------------+
	enum Layout
	{
		POS_NORMAL_TEX,                  // �ʒu�A�@���A�e�N�X�`��UV�����t�H�[�}�b�g
		POS_NORMAL_SKIN_TEX,             // �ʒu�A�@���A�X�L���p�̉e���{�[���Əd�ݏ��A�e�N�X�`��UV
		POS_NORMAL_TEX_TAN,              // �ʒu�A�@���x�N�g���A�e�N�X�`��UV�A�@���}�b�v�p�^���W�F���g���
		POS_NORMAL_SKIN_TEX_TAN          // �ʒu�A�@���A�X�L���p�̉e���{�[���Əd�ݏ��A�e�N�X�`��UV, �@���}�b�v�p�^���W�F���g���
	};

	VertexArray();
	VertexArray(const void* in_verts, unsigned int in_vertsNum, Layout in_layout, const unsigned int* in_inDices, unsigned int in_numInDices);     // ���_�z��R���X�g���N�^
	VertexArray(const float* in_verts, unsigned int in_vertsNum, const unsigned int* in_inDices, unsigned int in_numInDices);
	VertexArray(const float* in_verts, unsigned int in_vertsNum);
	
	void CreateCubeVerts();                      // �X�J�C�{�b�N�X�p���_�z��I�u�W�F�N�g�̍쐬
	void CreateScreenVerts();                    // �X�N���[���p���_�z��I�u�W�F�N�g�̍쐬
	void CreateSpriteVerts();                    // �X�v���C�g�p���_�z��I�u�W�F�N�g�̍쐬

	~VertexArray();                                                        // �f�X�g���N�^

	void SetActive();                                                      // ���̒��_�z����A�N�e�B�u�ɂ��ĕ`��Ŏg�p����

	unsigned int GetNumIndices() const { return m_numInDices; }            // �C���f�b�N�X�����擾
	unsigned int GetNumVerts() const { return m_vertsNum; }                // ���_�����擾

private:

	unsigned int m_vertsNum;                                               // ���_��
	unsigned int m_numInDices;                                             // �C���f�b�N�X�� (�|���S���ʐ�x3)
	unsigned int m_VAO;                                            // ���_�z��I�u�W�F�N�gID
	unsigned int m_VBO;                                           // ���_�o�b�t�@ID (OpenGL�ɓo�^���ɕt�^�����)
	unsigned int m_indexBuffer;                                              // �C���f�b�N�X�o�b�t�@ID (OpenGL�o�^���ɕt�^�����)
};