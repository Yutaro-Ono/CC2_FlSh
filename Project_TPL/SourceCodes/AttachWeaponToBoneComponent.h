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

	unsigned int m_boneNum;       // �A�^�b�`����{�[���ԍ�

	Vector3 m_adjustPos;          // �ʒu�������p�x�N�g��
	Vector3 m_adjustAngles;             // �e�����Ƃ̉�]�p�x

};