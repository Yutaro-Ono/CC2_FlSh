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

class AttachWeaponToBoneComponent : public Component
{

public:

	AttachWeaponToBoneComponent(class WeaponBase* _owner, class SkeletalMeshComponent* _skelMesh,
		                        unsigned int _boneNum);

	~AttachWeaponToBoneComponent();

	void Update(float in_deltaTime) override;

	void SetBoneNum(unsigned int _boneNum);

private:

	unsigned int m_socketNum;



};