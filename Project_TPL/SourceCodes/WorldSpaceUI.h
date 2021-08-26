//------------------------------------------------------------------+
// インターフェース基底クラス(ワールド空間用のUI)
//
//------------------------------------------------------------------+
#pragma once
#include "Math.h"
#include <string>

class WorldSpaceUI
{

public:

	WorldSpaceUI(const Vector3& _pos, const std::string& _filePath, float _scale = 1.0f);
	WorldSpaceUI(const Vector3& _pos, class Texture* _texture, float iscale = 1.0f);
	~WorldSpaceUI();


	void SetPosition(const Vector3& _pos) { m_position = _pos; }
	void SetTexture(class Texture* _tex) { m_texture = _tex; }
	int GetTextureID() { return m_textureID; }

	void Draw(class GLSLprogram* _shader);

	// 座標のセット
	void SetUIPosition(const Vector3& _pos);

	// ビルボード行列を定義・取得
	Matrix4 GetBillboardMatrix();

	// 表示非表示フラグのセッター
	void SetVisible(bool _visible) { m_isVisible = _visible; }

	void SetScale(float _scale) { m_scale = _scale; }

private:


	Vector3 m_position;                      // 座標

	float m_scale;                           // 拡大率

	class Texture* m_texture;                // テクスチャ
	int m_textureID;                         // テクスチャID

	bool m_isVisible;                        // 表示(false), 非表示(true)フラグ


	Matrix4 m_worldTransform;                // ワールド変換行列
	
	static Matrix4 m_staticBillboardMat;     // ビルボード行列 (カメラと平行になるように配置するための行列)


};