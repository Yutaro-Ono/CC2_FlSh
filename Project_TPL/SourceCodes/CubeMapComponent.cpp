#include "CubeMapComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "GLSLprogram.h"
#include "Math.h"
#include "SkyBox.h"
#include "VertexArray.h"
#include <vector>


CubeMapComponent::CubeMapComponent(Actor* _owner)
	:Component(_owner)
	,m_texture(nullptr)
	,m_intensity(1.0f)
	,m_isVisible(false)
{
}

CubeMapComponent::~CubeMapComponent()
{
	delete m_texture;
}

// �e�N�X�`���̐���
void CubeMapComponent::CreateTexture(const std::string& _filePath)
{
	m_texture = new Texture();
	m_texture->LoadCubeMap(_filePath);
}

// �L���[�u�}�b�v�̕`�揈��
void CubeMapComponent::Draw(GLSLprogram* _shader)
{
	// �����ɂ��Ă��Ȃ����
	if (m_isVisible)
	{
		// �[�x�ݒ�
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		// �e�N�X�`���o�C���h
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->GetTextureID());

		// uniform
		_shader->UseProgram();
		_shader->SetUniform("u_intensity", m_intensity);

		// �L���[�u�}�b�v�p���_�z��̃A�N�e�B�u��
		RENDERER->GetCubeMapVerts()->SetActive();
		// �`��
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// �O�̂��߃o�C���h����
		glBindVertexArray(0);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}

}

