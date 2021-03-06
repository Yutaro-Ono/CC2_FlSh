#include "LevelTerrain.h"
#include "Mesh.h"
#include "BoxColliderComponent.h"
#include "GameMain.h"
#include "Renderer.h"

LevelTerrain::LevelTerrain(Mesh* in_mesh, TERRAIN_TYPE in_type)
	:Actor(OBJECT_TAG::STATIC_OBJECT)
	,m_type(in_type)
{
	m_friction = SetTerrainFric(m_type);

	// メッシュをセット
	Mesh* mesh = in_mesh;
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// メッシュのカラーセット (マップ描画時に適用)
	if (m_type == TERRAIN_TYPE::TYPE_GLASS)
	{
		m_meshComp->SetMapColor(Vector3(0.3f, 0.41f, 0.4f));
	}
	if (m_type == TERRAIN_TYPE::TYPE_STREET)
	{
		m_meshComp->SetMapColor(Vector3(0.35f, 0.4f, 0.45f));
	}


	// 当たり判定設定
	m_box = new BoxColliderComponent(this);
	m_box->SetObjectBox(mesh->GetCollisionBox());
	
}

LevelTerrain::~LevelTerrain()
{
}

void LevelTerrain::UpdateActor(float in_deltaTime)
{
}

// 地形の摩擦力をセット
float LevelTerrain::SetTerrainFric(TERRAIN_TYPE in_type)
{
	if (in_type == TERRAIN_TYPE::TYPE_GLASS)
	{
		return 0.6f;
	}

	if (in_type == TERRAIN_TYPE::TYPE_STREET)
	{
		return 2.0f;
	}


	return 1.0f;
}
