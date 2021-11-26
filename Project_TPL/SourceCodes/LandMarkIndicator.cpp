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
	// �}�b�v���̃��f���ǂݍ���
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
	// �^�[�Q�b�g�A�N�^�[���Q�Ƃ��č��W�̍X�V/������]
	SetPosition(m_target->GetPosition());
	SetRotation(m_target->GetRotation());
	
}

/// <summary>
/// 
/// </summary>
/// <param name="_target"></param>
void LandMarkIndicator::SetTargetActor(Actor* _target)
{
	// �^�[�Q�b�g�A�N�^�[�̐؂�ւ�
	m_target = _target;
}
