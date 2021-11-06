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
	// 武器アクターがオーナーに追従する場合のみ座標調整
	if (m_weapon->GetExistsOwner())
	{
		// アタッチするボーンの指定と微調整用座標の更新
		//ChangeAttachBoneNum(10);
		//m_socketNum = 10;
		//SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));

		// ボーン行列 * 武器を持つプレイヤーのワールド行列で、ボーン用のワールド行列を作成
		Matrix4 boneWorldMat = m_skelMesh->GetBoneMat(m_boneNum) * m_weapon->GetOwnerPlayer()->GetWorldTransform();
		m_weapon->AdjustWorldMatToOwnerBone(boneWorldMat, _deltaTime);
	}

}

/// <summary>
/// 武器をアタッチするボーンの変更
/// </summary>
/// <param name="_boneNum"> ボーン番号 </param>
void AttachWeaponToBoneComponent::ChangeAttachBoneNum(unsigned int _boneNum)
{
	m_boneNum = _boneNum;
}

