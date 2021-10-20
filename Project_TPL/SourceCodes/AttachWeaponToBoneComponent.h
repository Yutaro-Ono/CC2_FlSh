//----------------------------------------------------------------------------------+
// @file        AttachWeaponToBoneComponent.h
// @brief       ����N���X���v���C���[�{�[���ɃA�^�b�`����R���|�[�l���g
// @note        �X�P���^�����b�V���̎w��{�[���ɃA�^�b�`����
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/20   �V�K�쐬
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