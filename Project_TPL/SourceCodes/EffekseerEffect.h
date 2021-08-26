//----------------------------------------------------------------------------------+
// @file        EffekseerEffect.h
// @brief       Effekseerを使用したエフェクトを再生
//              エフェクト情報の保持
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/26   新規作成
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

	bool Load(const char16_t* _filePath);     // エフェクトファイルの読み込み処理


	// Getter/Setter
	bool GetIsReady() { return m_isReady; }

	Effekseer::Handle CreateInstanceHandle(Vector3& _pos);

	static void SetPosition(Vector3& _pos, Effekseer::Handle _handle);
	static void SetRotation(Vector3& _axis, float _angle, Effekseer::Handle _handle);
	static void SetBaseMatrix(Matrix4& _mat, Effekseer::Handle _handle);


private:

	bool                   m_isReady;       // エフェクトが正常に読み込めたか

	Effekseer::EffectRef   m_effectRef;     // エフェクト本体
	Effekseer::Handle      m_handle;        // エフェクト再生ハンドル

};