#include "EffekseerEffect.h"
#include "GameMain.h"
#include "Renderer.h"
#include <EffekseerRendererGL.h>
#include <iostream>

EffekseerEffect::EffekseerEffect()
    :m_effectRef(nullptr)
    ,m_handle(-1)
    ,m_isReady(false)
{
}

EffekseerEffect::~EffekseerEffect()
{
}

/// <summary>
/// �G�t�F�N�g�t�@�C���̓ǂݍ��ݏ���
/// </summary>
/// <param name="_filePath"> �G�t�F�N�g�t�@�C���̃p�X </param>
/// <returns> �ǂݍ��݊����Ftrue | �ǂݍ��ݎ��s�Ffalse </returns>
bool EffekseerEffect::Load(const char16_t* _filePath)
{
	// �G�t�F�N�g�p��Effekseer�}�l�[�W���𐶐��E�擾
	m_effectRef = Effekseer::Effect::Create(RENDERER->GetEffekseerManager(), _filePath);
	
	if (m_effectRef == nullptr)
	{
		std::cout << "ERROR::" << _filePath << " ::Load Failed" << std::endl;
		m_isReady = false;
	}
	else
	{
		std::cout << _filePath << " ::Load Success." << std::endl;
		m_isReady = true;
	}

	return m_isReady;
}

/// <summary>
/// �G�t�F�N�g�Đ��p�̃n���h���𐶐���
/// �w��̍��W�ŃG�t�F�N�g���Đ�����
/// </summary>
/// <param name="_pos"> �G�t�F�N�g�̍��W </param>
/// <returns> �G�t�F�N�g�Đ��n���h�� </returns>
Effekseer::Handle EffekseerEffect::CreateInstanceHandle(Vector3& _pos)
{
	// �ǂݍ��݂��������Ă��鎞�̂�
	if (!m_isReady)
	{
		return 0;
	}

	// �n���h���ɍĐ�����G�t�F�N�g������
	m_handle = RENDERER->GetEffekseerManager()->Play(m_effectRef, Effekseer::Vector3D(_pos.x, _pos.y, _pos.z));

	return m_handle;
}

/// <summary>
/// �G�t�F�N�g���Đ�������W�̃Z�b�g
/// </summary>
/// <param name="_pos"> �Đ����������W </param>
/// <param name="_handle"> �ύX����G�t�F�N�g�n���h�� </param>
void EffekseerEffect::SetPosition(Vector3& _pos, Effekseer::Handle _handle)
{
	Effekseer::Vector3D v;
	v = _pos;
	RENDERER->GetEffekseerManager()->SetLocation(_handle, v);
}

/// <summary>
/// �G�t�F�N�g�̉�]�l���Z�b�g
/// </summary>
/// <param name="_axis"> ��]�� </param>
/// <param name="_angle"> ��]�p </param>
/// <param name="_handle"> �ύX����G�t�F�N�g�n���h�� </param>
void EffekseerEffect::SetRotation(Vector3& _axis, float _angle, Effekseer::Handle _handle)
{
	Effekseer::Vector3D v;
	v = _axis;
	RENDERER->GetEffekseerManager()->SetRotation(_handle, v, _angle);
}

/// <summary>
/// �G�t�F�N�g�̃x�[�X�ƂȂ郏�[���h�s����Z�b�g
/// </summary>
/// <param name="_mat"> �x�[�X�s�� </param>
/// <param name="_handle"> �ύX����G�t�F�N�g�n���h�� </param>
void EffekseerEffect::SetBaseMatrix(Matrix4& _mat, Effekseer::Handle _handle)
{
	Effekseer::Matrix43 m;
	m = _mat;
	RENDERER->GetEffekseerManager()->SetBaseMatrix(_handle, m);
}
