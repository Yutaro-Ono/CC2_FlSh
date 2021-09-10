//----------------------------------------------------------------------------------+
// @file        Vehicle.h
// @brief       ��蕨�x�[�X�N���X�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/ 7   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"
#include "Tag.h"
#include "Math.h"
#include <vector>

class Vehicle : public Actor
{

public:

	Vehicle(OBJECT_TAG _tag);
	virtual ~Vehicle();

	void ActivateRideMode(class Actor* _rideActor);
	void DeactivateRideMode();

protected:



	class Actor*    m_rideActor;                     // ����Ă���A�N�^�[
	Vector3         m_rideOffset;                    // ����Ă���A�N�^�[�̍��W�����p�x�N�g��

	bool            m_accel;                         // �A�N�Z���y�_���𓥂�ł��邩
	bool            m_brake;                         // �u���[�L�y�_���𓥂�ł��邩
	float           m_speed;                         // ��蕨�̃X�s�[�h
	float           m_velocity;                      // �����x
	float           m_weight;                        // ��蕨�̏d��
	float           m_friction;                      // ���C�W��

	bool            m_isRide;                        // �v���C���[�ɏ���Ă��邩�ǂ���

};