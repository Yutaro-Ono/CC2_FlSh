#include "BulletActor.h"
#include "ColliderComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"

BulletActor::BulletActor(const Vector3& _pos, const Vector3& _shootDirVec)
	:Actor(OBJECT_TAG::ATTACK_BULLET)
	,m_dirVec(_shootDirVec)
	,m_velocity(100.0f)
{
	m_position = _pos;

	// ���b�V���̐���
	Mesh* mesh = RENDERER->GetMesh("");
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

}

BulletActor::~BulletActor()
{
}

void BulletActor::UpdateActor(float _deltaTime)
{
	Vector3 shootVec = m_dirVec * m_velocity * _deltaTime;
	SetPosition(shootVec);

}

void BulletActor::OnCollisionEnter(ColliderComponent* _ownCollComp, ColliderComponent* _otherCollComp)
{
	// �^�O���Ƃɏ����𕪊�
	OBJECT_TAG otherTag = _otherCollComp->GetOwnerTag();

	// �G(�]���r)�ɓ�������
	if (otherTag == OBJECT_TAG::ACTOR_ENEMY)
	{
		
	}

	// �n�`�ɓ��������ꍇ�͒e������
	if (otherTag == OBJECT_TAG::STATIC_OBJECT)
	{
		m_state = ACTOR_STATE::STATE_DEAD;
	}

}
