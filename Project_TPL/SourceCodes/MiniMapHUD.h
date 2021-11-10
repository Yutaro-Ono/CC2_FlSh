#pragma once
#include <vector>
#include "Math.h"

class MiniMapHUD
{

public:

	MiniMapHUD(class Actor* _target);
	~MiniMapHUD();

	void WriteBuffer(class GLSLprogram* _shader, std::vector<class MeshComponent*> _mesh);

	void Draw(class GLSLprogram* _shader);


private:

	void CreateFBO(unsigned int& _fbo);

	unsigned int m_miniMapFBO;
	unsigned int m_mapBuffer;
	unsigned int m_rbo;

	class Actor* m_target;                 // ターゲット
	class Texture* m_mapTex;               // マップテクスチャ (円形)

	class LandMarkIndicator* m_landMark;

	int m_width;
	int m_height;
	float m_scale;                         // マップUIスケール

	Vector3 m_viewPos;                     // ビュー座標 (ここを起点にマップ描画)

	Vector2 m_screenPos;                   // スクリーン上の座標

	Matrix4 m_projection;
	//Matrix4 m_lowerViewProj;             // 下向きのビュー&プロジェクション合成行列

};