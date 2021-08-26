//----------------------------------------------------------------------------------+
// @file        EffekseerEffect.h
// @brief       Effekseer���g�p�����G�t�F�N�g���Đ�
//              �G�t�F�N�g���̕ێ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/26   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include <Effekseer.h>
#include "Math.h"

class EffekseerEffect
{

public:

	EffekseerEffect();
	~EffekseerEffect();

	bool Load(const char16_t* _filePath);     // �G�t�F�N�g�t�@�C���̓ǂݍ��ݏ���


	// Getter/Setter
	bool GetIsReady() { return m_isReady; }

	Effekseer::Handle CreateInstanceHandle(Vector3& _pos);

	static void SetPosition(Vector3& _pos, Effekseer::Handle _handle);
	static void SetRotation(Vector3& _axis, float _angle, Effekseer::Handle _handle);
	static void SetBaseMatrix(Matrix4& _mat, Effekseer::Handle _handle);


private:

	bool                   m_isReady;       // �G�t�F�N�g������ɓǂݍ��߂���

	Effekseer::EffectRef   m_effectRef;     // �G�t�F�N�g�{��
	Effekseer::Handle      m_handle;        // �G�t�F�N�g�Đ��n���h��

};