#include "WeaponAR4.h"
#include "Mesh.h"
#include "MeshComponent.h"

const std::string WeaponAR4::AR4_MESH_PATH = "Data/Meshes/Weapons/AR4/SM_AR4.gpmesh";

WeaponAR4::WeaponAR4()
{
	Initialize();
}

WeaponAR4::WeaponAR4(Actor* _owner)
{
	// オーナーアクターをセット
	m_existsOwner = true;
	m_owner = _owner;

	Initialize();
}

WeaponAR4::~WeaponAR4()
{
}

void WeaponAR4::Initialize()
{
	// AR4のメッシュをロード
	Mesh* mesh = RENDERER->GetMesh(AR4_MESH_PATH);
	
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
}

