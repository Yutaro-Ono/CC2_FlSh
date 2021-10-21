#include "AttachWeaponToBoneComponent.h"
#include "WeaponBase.h"
#include "Player.h"
#include "SkeletalMeshComponent.h"

AttachWeaponToBoneComponent::AttachWeaponToBoneComponent(WeaponBase* _owner, SkeletalMeshComponent* _skelMesh, unsigned int _boneNum)
	:Component(_owner)
	,m_weapon(_owner)
	,m_skelMesh(_skelMesh)
	,m_boneNum(_boneNum)
	,m_adjustPos(Vector3::Zero)
	,m_adjustAngles(Vector3::Zero)
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
		// �{�[���s�� * ��������v���C���[�̃��[�J�����W�s��ŁA�{�[���p�̃��[���h�s����쐬
		Matrix4 boneLocalMat = m_skelMesh->GetBoneMat(m_boneNum) * Matrix4::CreateTranslation(m_weapon->GetOwnerPlayer()->GetPosition());
		m_weapon->AdjustWorldMatToOwnerBone(boneLocalMat, _deltaTime);
	}

}

/// <summary>
/// ������A�^�b�`����{�[���̕ύX�ƁA����ɔ��������p���W�̍X�V
/// </summary>
/// <param name="_boneNum"> �{�[���ԍ� </param>
/// <param name="_adjustPos"> �ʒu�������p�x�N�g�� </param>
void AttachWeaponToBoneComponent::ChangeAttachBoneNum(unsigned int _boneNum, const Vector3& _adjustPos)
{
	m_boneNum = _boneNum;
	m_adjustPos = _adjustPos;
}

/// <summary>
/// �����p�̉�]�l�Z�b�^�[
/// </summary>
/// <param name="_angles"> Vector3(x,y,z�͊e����\��) </param>
void AttachWeaponToBoneComponent::SetAdjustAngles(const Vector3& _angles)
{
	m_adjustAngles = _angles;
}
