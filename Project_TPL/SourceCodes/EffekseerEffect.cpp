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
/// エフェクトファイルの読み込み処理
/// </summary>
/// <param name="_filePath"> エフェクトファイルのパス </param>
/// <returns> 読み込み完了：true | 読み込み失敗：false </returns>
bool EffekseerEffect::Load(const char16_t* _filePath)
{
	// エフェクト用のEffekseerマネージャを生成・取得
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
/// エフェクト再生用のハンドルを生成し
/// 指定の座標でエフェクトを再生する
/// </summary>
/// <param name="_pos"> エフェクトの座標 </param>
/// <returns> エフェクト再生ハンドル </returns>
Effekseer::Handle EffekseerEffect::CreateInstanceHandle(Vector3& _pos)
{
	// 読み込みが完了している時のみ
	if (!m_isReady)
	{
		return 0;
	}

	// ハンドルに再生するエフェクト情報を代入
	m_handle = RENDERER->GetEffekseerManager()->Play(m_effectRef, Effekseer::Vector3D(_pos.x, _pos.y, _pos.z));

	return m_handle;
}

/// <summary>
/// エフェクトを再生する座標のセット
/// </summary>
/// <param name="_pos"> 再生したい座標 </param>
/// <param name="_handle"> 変更するエフェクトハンドル </param>
void EffekseerEffect::SetPosition(Vector3& _pos, Effekseer::Handle _handle)
{
	Effekseer::Vector3D v;
	v = _pos;
	RENDERER->GetEffekseerManager()->SetLocation(_handle, v);
}

/// <summary>
/// エフェクトの回転値をセット
/// </summary>
/// <param name="_axis"> 回転軸 </param>
/// <param name="_angle"> 回転角 </param>
/// <param name="_handle"> 変更するエフェクトハンドル </param>
void EffekseerEffect::SetRotation(Vector3& _axis, float _angle, Effekseer::Handle _handle)
{
	Effekseer::Vector3D v;
	v = _axis;
	RENDERER->GetEffekseerManager()->SetRotation(_handle, v, _angle);
}

/// <summary>
/// エフェクトのベースとなるワールド行列をセット
/// </summary>
/// <param name="_mat"> ベース行列 </param>
/// <param name="_handle"> 変更するエフェクトハンドル </param>
void EffekseerEffect::SetBaseMatrix(Matrix4& _mat, Effekseer::Handle _handle)
{
	Effekseer::Matrix43 m;
	m = _mat;
	RENDERER->GetEffekseerManager()->SetBaseMatrix(_handle, m);
}
