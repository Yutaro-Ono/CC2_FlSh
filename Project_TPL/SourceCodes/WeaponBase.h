//----------------------------------------------------------------------------------+
// @file        WeaponBase.h
// @brief       ����x�[�X�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/12   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class WeaponBase : public Actor
{

public:

	WeaponBase();
	~WeaponBase();

	void UpdateActor(float _deltaTime) override;

	void SetSocketNum(size_t _socketNum) { m_socketNum = _socketNum; }

protected:

	void SetOwnerActor(class Actor* _owner);
	void RemoveOwnerActor();

	void SetSocketMat(Matrix4 _socketMat);



	class Actor* m_owner;              // ���̃N���X�����L����A�N�^�[�|�C���^
	bool m_existsOwner;                // ���̕���N���X�����L����I�[�i�[�A�N�^�\�͑��݂��邩

	Matrix4 m_socketMat;               // ���̕���I�u�W�F�N�g���X�i�b�v����I�[�i�[�̃\�P�b�g�s��
	size_t m_socketNum;                 // �{�[���̃\�P�b�g�ԍ�

	unsigned int m_attackInterval;     // �U���̊Ԋu(�~���b�P��)
	unsigned int m_attackStartCount;   // �U���J�n���̃J�E���g
	unsigned int m_attackNowCount;     // �U���J�E���g�X�V�p�ϐ�(attackStartCount�Ɣ�r)

	// �e�ۊ֌W�ϐ�
	unsigned int m_ammoCount;          // ���݂̃}�K�W���ɓ����Ă�e���
	unsigned int m_ammoBullet;         // 1�}�K�W���ɓ���e���
	unsigned int m_ammoMax;            // �����ł���ő�e���
	bool m_isMelee;                    // �ߐڕ��킩�ǂ���


};