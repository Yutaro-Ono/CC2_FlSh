//----------------------------------------------------------------------------------+
// @file        PlayerActor.h
// @brief       プレイヤーアクタクラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   新規作成
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