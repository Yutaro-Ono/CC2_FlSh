//----------------------------------------------------------------------------------+
// @file        PlayerActor.h
// @brief       �v���C���[�A�N�^�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"

enum class PLAYER_STATE
{
	STATE_IDLE,
	STATE_WALK,
	STATE_RUN,
	STATE_JUMP_START,
	STATE_JUMP_LOOP,
	STATE_JUMP_LAND,


	STATE_ALL_NUM
};

class Player : public Actor
{

public:


	Player(OBJECT_TAG _tag);
	~Player();

	void UpdateActor(float _deltaTime) override;

private:



};