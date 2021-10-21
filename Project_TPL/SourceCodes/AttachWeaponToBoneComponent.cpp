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
	// 武器アクターがオーナーに追従する場合のみ座標調整
	if (m_weapon->GetExistsOwner())
	{
		// ボーン行列 * 武器を持つプレイヤーのローカル座標行列で、ボーン用のワールド行列を作成
		Matrix4 boneLocalMat = m_skelMesh->GetBoneMat(m_boneNum) * Matrix4::CreateTranslation(m_weapon->GetOwnerPlayer()->GetPosition());
		m_weapon->AdjustWorldMatToOwnerBone(boneLocalMat, _deltaTime);
	}

}

/// <summary>
/// 武器をアタッチするボーンの変更と、それに伴う調整用座標の更新
/// </summary>
/// <param name="_boneNum"> ボーン番号 </param>
/// <param name="_adjustPos"> 位置微調整用ベクトル </param>
void AttachWeaponToBoneComponent::ChangeAttachBoneNum(unsigned int _boneNum, const Vector3& _adjustPos)
{
	m_boneNum = _boneNum;
	m_adjustPos = _adjustPos;
}

/// <summary>
/// 調整用の回転値セッター
/// </summary>
/// <param name="_angles"> Vector3(x,y,zは各軸を表す) </param>
void AttachWeaponToBoneComponent::SetAdjustAngles(const Vector3& _angles)
{
	m_adjustAngles = _angles;
}
