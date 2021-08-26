//-----------------------------------------------------------------------+
// メッシュコンポーネントクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <cstddef>
#include "Math.h"

class MeshComponent : public Component
{
public:

	MeshComponent(class Actor* _owner, bool _isSkeletal = false);       // コンストラクタ(親アクターが必要)
	~MeshComponent();                                                       // デストラクタ

	virtual void Draw(class GLSLprogram* _shader);                             // メッシュコンポーネントの描画
	virtual void DrawShadow(class GLSLprogram* _shader);                       // シャドウ生成用の描画
	void DrawMap(class GLSLprogram* _shader);                                  // マップHUD描画用

	virtual void SetMesh(class Mesh* _mesh) { m_mesh = _mesh; }         // メッシュコンポーネントで使用するメッシュのセット

	void SetTexturesToUniform(class GLSLprogram* _shader);                     // シェーダuniformにテクスチャをセットする関数
	void SetTextureIndex(size_t _index) { m_textureIndex = _index; }    // テクスチャのセット

	void SetVisible(bool _visible) { m_visible = _visible; }            // 表示フラグのセッター
	bool GetVisible() const { return m_visible; }                           // 表示フラグのゲッター

	bool GetIsSkeletal() const { return m_isSkeletal; }                     // スケルタルかどうかのゲッター

	void SetIntensityVal(float _val) { m_intensity = _val; }            // 輝度強度のセッター

	void SetDrawMap(bool _flg) { m_isDrawMap = _flg; }
	void SetMapColor(const Vector3& _color) { m_mapColor = _color; }    // マップ描画用のカラーセッター

protected:

	class Mesh* m_mesh;                                                     // メッシュオブジェクトの格納用
	size_t m_textureIndex;                                                  // テクスチャーのインデックス

	bool m_visible;                                                         // 表示フラグ
	bool m_isDrawMap;                                                       // マップ画面に表示するか
	bool m_isSkeletal;                                                      // スケルタルモデルかどうか

	float m_intensity;                                                      // 輝度の強度情報 (エミッシブマップを適用しているメッシュのみ)

	Vector3 m_mapColor;                                                     // マップ描画時のカラー情報
};