//----------------------------------------------------------------------------------+
// @file        VertexArray.h
// @brief       ���_�z���VAO/VBO�ɓo�^����
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "VertexArray.h"


VertexArray::VertexArray()
{
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_verts"> ���_�z�� </param>
/// <param name="_vertsNum"> ���_�� </param>
/// <param name="_layout"> ���_���C�A�E�g�̎�� </param>
/// <param name="_inDices"> �C���f�b�N�X�o�b�t�@ </param>
/// <param name="_numInDices"> �C���f�b�N�X�� </param>
VertexArray::VertexArray(const void* _verts, unsigned int _vertsNum, VERTEX_LAYOUT::TYPE _layout, const unsigned int* _inDices, unsigned int _numInDices)
	:m_vertsNum(_vertsNum)
	,m_numInDices(_numInDices)
{
	// ���_�z��I�u�W�F�N�g�̓o�^�E�o�C���h
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// ���_�f�[�^�T�C�Y�̌v�Z (�v�Z���ɂ��Ă�Layout�̒�`�̕����ɖ��L)
	unsigned vertexSize = 8 * sizeof(float);    // POS_NORMAL_UV

	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN)
	{
		vertexSize = 11 * sizeof(float);
	}
	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}
	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
	{
		vertexSize = 11 * sizeof(float) + 8 * sizeof(char);
	}

	// ���_�o�b�t�@�I�u�W�F�N�g�̐���
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertsNum * vertexSize, _verts, GL_STATIC_DRAW);

	// �C���f�b�N�X�o�b�t�@�̐���
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numInDices * sizeof(unsigned int), _inDices, GL_STATIC_DRAW);

	// �A�g���r���[�g�Z�b�g ����
	// float 3���@���@�ʒu x,y,z�@�ʒu�������Z�b�g
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
	// ����float 3�� �� �@�� nx, ny, nz�@�@���������Z�b�g
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		(void*)(3 * sizeof(float)));
	// ���_�������Ƃɏ�����h��
	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV)
	{
		// ����float 2�� u, v  �e�N�X�`�����W�������Z�b�g
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			(void*)(6 * sizeof(float)));
	}
	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN)
	{
		// 2 : �e�N�X�`�����W
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(6 * sizeof(float)));
		// 3 : �ڃx�N�g�� (�^���W�F���g)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(8 * sizeof(float)));
	}
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_verts"> ���_�z�� </param>
/// <param name="_vertsNum"> ���_�� </param>
/// <param name="_inDices"> �C���f�b�N�X�o�b�t�@ </param>
/// <param name="_numInDices"> �C���f�b�N�X�� </param>
VertexArray::VertexArray(const float* _verts, unsigned int _vertsNum, const unsigned int* _inDices, unsigned int _numInDices)
{
}


VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_indexBuffer);
	glDeleteVertexArrays(1, &m_vao);
}

/// <summary>
/// ���_�z��I�u�W�F�N�g���o�C���h���A�`��̏���������
/// </summary>
void VertexArray::SetActive()
{
	glBindVertexArray(m_vao);
}

/// <summary>
/// �V���v���Ȏl�p�` (����)
/// �X�N���[���Ɏg�p
/// </summary>
void VertexArray::CreateSimpleQuadVAO()
{
	float quadVertices[] =
	{
		// �|�W�V����   // �e�N�X�`�����W
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

/// <summary>
/// �L���[�u�}�b�v�p���_�z��̍쐬
/// </summary>
void VertexArray::CreateCubeMapVertices()
{
	float cubeMapVertices[] = 
	{      
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int indices[] =
	{
		0,  1,  2,  0,  2,  3,    // �O��
		4,  5,  6,  4,  6,  7,    // �w��
		8,  9, 10,  8, 10, 11,    // ���
		12, 13, 14, 12, 14, 15,    // ���
		16, 17, 18, 16, 18, 19,    // �E����
		20, 21, 22, 20, 22, 23     // ������
	};


	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * 8 * sizeof(float), cubeMapVertices, GL_STATIC_DRAW);
	// �A�g���r���[�g�ւ̃Z�b�g(���_���W�̂�)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}