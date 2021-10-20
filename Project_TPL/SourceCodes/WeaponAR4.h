//----------------------------------------------------------------------------------+
// @file        WeaponAR4.h
// @brief       ����A�N�^�[�FAR4
// @note        AR4�̏�������
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/12   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "WeaponBase.h"

class WeaponAR4 : public WeaponBase
{

public:

	WeaponAR4();
	WeaponAR4(class Actor* _owner);
	WeaponAR4(class Player* _owner);
	~WeaponAR4();


private:

	void Initialize();

protected:

	void UpdateSocketMat(float _deltaTime) override;


	static const unsigned int SOCKET_NUM_SPINE;
	static const unsigned int SOCKET_NUM_RIGHTHAND;
	static const unsigned int SOCKET_NUM_LEFTHAND;

	class FirstPersonCameraComponent* m_fpsCamera;

	static const Vector3 ADJUST_POS_BASIC;
	static const Vector3 ADJUST_POS_IDLE_WEAPONOUT;
	static const Vector3 ADJUST_POS_MOVE_WEAPONOUT;

	static const std::string AR4_MESH_PATH;

};