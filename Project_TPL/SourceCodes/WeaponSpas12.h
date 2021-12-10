#pragma once
#include "WeaponBase.h"

class WeaponSpas12 : public WeaponBase
{

public:

	WeaponSpas12();
	WeaponSpas12(class Actor* _owner);
	WeaponSpas12(class Player* _owner);
	~WeaponSpas12();

	void AdjustWorldMatToOwnerBone(const Matrix4& _boneWorldMat, float _deltaTime) override;

private:

	void Initialize();

	Vector3 m_adjustHolsteredIdlePos;          // ����g�s/�ҋ@���̒������W
	Vector3 m_adjustUnholsteredIdlePos;        // ����o��/�ҋ@���̒������W
	Vector3 m_adjustUnholsteredMovePos;        // ����o��/�ړ����̒������W
	Vector3 m_adjustAimPos;                    // �G�C�����̒������W

	Quaternion m_holsterIdleRot;
	float m_holsterRadianY;

	Quaternion m_unholsterIdleRot;
	float m_unholsterIdleRadianX;
	float m_unholsterIdleRadianY;
	float m_unholsterIdleRadianZ;

	Quaternion m_unholsterMoveRot;
	float m_unholsterMoveRadianX;
	float m_unholsterMoveRadianY;
	float m_unholsterMoveRadianZ;

protected:



	static const unsigned int SOCKET_NUM_SPINE;
	static const unsigned int SOCKET_NUM_RIGHTHAND;
	static const unsigned int SOCKET_NUM_LEFTHAND;

	class FirstPersonCameraComponent* m_fpsCamera;

	static const Vector3 ADJUST_POS_BASIC;
	static const Vector3 ADJUST_POS_IDLE_WEAPONOUT;
	static const Vector3 ADJUST_POS_MOVE_WEAPONOUT;

	static const std::string SPAS12_MESH_PATH;

};