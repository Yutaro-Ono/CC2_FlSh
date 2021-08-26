#pragma once
#include "Component.h"

class CarMeshComponent : public Component
{

public:

	CarMeshComponent(class Actor* _owner);
	~CarMeshComponent();


	virtual void Draw(class GLSLprogram* _shader);                             // メッシュコンポーネントの描画
	virtual void DrawShadow(class GLSLprogram* _shader);                       // シャドウ生成用の描画

	virtual void SetMesh(class Mesh* _mesh) { m_mesh = _mesh; }         // メッシュコンポーネントで使用するメッシュのセット

	void SetTextureIndex(size_t _index) { m_textureIndex = _index; }    // テクスチャのセット

	void SetReflect(bool _ref) { m_reflect = _ref; }                    // 反射計算の有無

	void SetVisible(bool _visible) { m_visible = _visible; }            // 表示フラグのセッター
	bool GetVisible() const { return m_visible; }                           // 表示フラグのゲッター



protected:

	class Mesh* m_mesh;                                                     // メッシュオブジェクトの格納用
	size_t m_textureIndex;                                                  // テクスチャーのインデックス

	bool m_reflect;                                                         // 反射計算を行うか

	bool m_visible;                                                         // 表示フラグ
};