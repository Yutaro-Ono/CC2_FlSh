//----------------------------------------------------------------------------------+
// @file        DirectionalLightDebug.h
// @brief       ディレクショナルライトのデバッグクラス
// @note        DebugObjectBaseクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   新規作成
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