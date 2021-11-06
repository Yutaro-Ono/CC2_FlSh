#include "AttachWeaponToBoneComponent.h"
#include "WeaponBase.h"
#include "Player.h"
#include "SkeletalMeshComponent.h"

AttachWeaponToBoneComponent::AttachWeaponToBoneComponent(WeaponBase* _owner, SkeletalMeshComponent* _skelMesh, unsigned int _boneNum)
	:Component(_owner)
	,m_weapon(_owner)
	,m_skelMesh(_skelMesh)
	,m_boneNum(_boneNum)
{
}

AttachWeaponToBoneComponent::~AttachWeaponToBoneComponent()
{
}

void AttachWeaponToBoneComponent::Update(float _deltaTime)
{
	// ����A�N�^�[���I�[�i�[�ɒǏ]����ꍇ�̂ݍ��W����
	if (m_weapon->GetExistsOwner())
	{
		// �A�^�b�`����{�[���̎w��Ɣ������p���W�̍X�V
		//ChangeAttachBoneNum(10);
		//m_socketNum = 10;
		//SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));

		// �{�[���s�� * ��������v���C���[�̃��[���h�s��ŁA�{�[���p�̃��[���h�s����쐬
		Matrix4 boneWorldMat = m_skelMesh->GetBoneMat(m_boneNum) * m_weapon->GetOwnerPlayer()->GetWorldTransform();
		m_weapon->AdjustWorldMatToOwnerBone(boneWorldMat, _deltaTime);
	}

}

/// <summary>
/// ������A�^�b�`����{�[���̕ύX
/// </summary>
/// <param name="_boneNum"> �{�[���ԍ� </param>
void AttachWeaponToBoneComponent::ChangeAttachBoneNum(unsigned int _boneNum)
{
	m_boneNum = _boneNum;
}

