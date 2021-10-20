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
	//// ローカルボーン行列 * 武器を持つプレイヤーのワールド行列で、ボーン用のワールド行列を作成
	//Matrix4 worldBoneMat = m_skelMesh->GetBoneMat(m_boneNum) * Matrix4::CreateTranslation(m_weapon->GetOwnerPlayer()->GetPosition());

	//// スケール→調整座標→回転値→ボーンのワールド行列の順で計算
	//Matrix4 resultWorldMat = Matrix4::CreateScale(m_weapon->GetScale())
	//	* Matrix4::CreateTranslation(m_adjustPos)
	//	//* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(m_adjustAngles.x)))
	//	* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(m_adjustAngles.z)))
	//	* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(m_adjustAngles.y)))
	//	* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(-80.0f)))
	//	* worldBoneMat;

	//// 武器クラスに結果のワールド行列をセット
	//m_weapon->SetWorldTransform(resultWorldMat);

	m_weapon->UpdateSocketMat(_deltaTime);
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
