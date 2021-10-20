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
	//// ���[�J���{�[���s�� * ��������v���C���[�̃��[���h�s��ŁA�{�[���p�̃��[���h�s����쐬
	//Matrix4 worldBoneMat = m_skelMesh->GetBoneMat(m_boneNum) * Matrix4::CreateTranslation(m_weapon->GetOwnerPlayer()->GetPosition());

	//// �X�P�[�����������W����]�l���{�[���̃��[���h�s��̏��Ōv�Z
	//Matrix4 resultWorldMat = Matrix4::CreateScale(m_weapon->GetScale())
	//	* Matrix4::CreateTranslation(m_adjustPos)
	//	//* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(m_adjustAngles.x)))
	//	* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(m_adjustAngles.z)))
	//	* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(m_adjustAngles.y)))
	//	* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(-80.0f)))
	//	* worldBoneMat;

	//// ����N���X�Ɍ��ʂ̃��[���h�s����Z�b�g
	//m_weapon->SetWorldTransform(resultWorldMat);

	m_weapon->UpdateSocketMat(_deltaTime);
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
