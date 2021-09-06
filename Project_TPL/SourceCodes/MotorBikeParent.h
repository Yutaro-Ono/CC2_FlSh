//----------------------------------------------------------------------------------+
// @file        MotorBikeParent.h
// @brief       �o�C�N(��蕨)�e�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/ 6   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"
#include <vector>


class MotorBikeParent : public Actor
{

public:

	enum class MOTORBIKE_PARTS
	{
		BODY = 0,
		HANDLE,
		FRONT_WHEEL,
		BACK_WHEEL,
		ALL
	};

	MotorBikeParent();
	~MotorBikeParent();

	void UpdateActor(float _deltaTime) override;




private:

	// �e�p�[�c���A�N�^�[�Œ�`
	class Actor* m_parts[static_cast<int>(MOTORBIKE_PARTS::ALL)];



};