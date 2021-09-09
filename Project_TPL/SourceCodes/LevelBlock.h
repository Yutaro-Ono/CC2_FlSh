//-----------------------------------------------------------------------+
// ステージ用ブロッククラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"
#include "MeshComponent.h"

class LevelBlock : public Actor
{


public:

	LevelBlock();                            // コンストラクタ
	~LevelBlock();                           // デストラクタ


	void SetMesh(class Mesh* in_mesh);       // メッシュのセット
	void SetMeshVisible() { m_meshComp->SetVisible(false); }     // メッシュの透明化
	void SetMeshIntensity(float _intensity) { m_meshComp->SetIntensityVal(_intensity); }

	void UpdateActor(float in_deltaTime);

private:


	class BoxCollider* m_box;
};