//----------------------------------------------------------------------------------+
// @file        AttachWeaponToBoneComponent.h
// @brief       武器クラスをプレイヤーボーンにアタッチするコンポーネント
// @note        スケルタルメッシュの指定ボーンにアタッチする
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/20   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <string>
#include "Math.h"

class AttachWeaponToBoneComponent : public Component
{

public:

	AttachWeaponToBoneComponent(class WeaponBase* _owner, class SkeletalMeshComponent* _skelMesh,
		                        unsigned int _boneNum);

	~AttachWeaponToBoneComponent();

	void Update(float _deltaTime) override;

	void ChangeAttachBoneNum(unsigned int _boneNum, const Vector3& _adjustPos);

	void SetAdjustAngles(const Vector3& _angles);

private:

	class WeaponBase* m_weapon;
	class SkeletalMeshComponent* m_skelMesh;

	unsigned int m_boneNum;       // アタッチするボーン番号

	Vector3 m_adjustPos;          // 位置微調整用ベクトル
	Vector3 m_adjustAngles;             // 各軸ごとの回転角度

};