//----------------------------------------------------------------------------------+
// @file        DirectionalLightDebug.h
// @brief       �f�B���N�V���i�����C�g�̃f�o�b�O�N���X
// @note        DebugObjectBase�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "DebugObjectBase.h"

class DirectionalLightDebug : public DebugObjectBase
{

public:

	DirectionalLightDebug(class DirectionalLight* _owner);
	~DirectionalLightDebug();

	void Update(float _deltaTime) override;
	void Render() override;


private:

	class DirectionalLight* m_owner;

	bool m_diffuseColorPick;
	bool m_specularColorPick;
	bool m_ambientColorPick;

};