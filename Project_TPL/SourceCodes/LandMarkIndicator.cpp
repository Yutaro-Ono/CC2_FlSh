#include "LandMarkIndicator.h"
#include "PlayerManager.h"
#include "PlayerHuman.h"
#include "PlayerCar.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "WorldSpaceUI.h"
#include "CameraComponent.h"

LandMarkIndicator::LandMarkIndicator(Actor* in_player)
	:Actor(OBJECT_TAG::PARTICLE)
	,m_target(in_player)
{
	SetScale(750.0f);
	// マップ矢印のモデル読み込み
	Mesh* mesh = RENDERER->GetMesh("Data/Meshes/Objects/Interface/Arrow/MapHUD_Arrow.OBJ");
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
	m_meshComp->SetVisible(false);
	m_meshComp->SetMapColor(Vector3(1.0f, 0.1f, 0.6f));
}


LandMarkIndicator::~LandMarkIndicator()
{
}

void LandMarkIndicator::UpdateActor(float in_deltaTime)
{
	// ターゲットアクターを参照して座標の更新/矢印を回転
	SetPosition(m_target->GetPosition());
	SetRotation(m_target->GetRotation());
	
}

/// <summary>
/// 
/// </summary>
/// <param name="_target"></param>
void LandMarkIndicator::SetTargetActor(Actor* _target)
{
	// ターゲットアクターの切り替え
	m_target = _target;
}
