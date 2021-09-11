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
	STATE_JOG,
	STATE_RUN,

	STATE_ALL_NUM
};

class Player : public Actor
{

public:


	Player();
	~Player();

	void UpdateActor(float _deltaTime) override;

	class SkeletalMeshComponent* GetSkelMesh() { return m_skelComp; }
	const class Animation* GetAnim(PLAYER_STATE _state) { return m_anims[static_cast<unsigned int>(_state)]; }

private:

	void UpdatePlayerState(float _deltaTime);


	// ステート制御用
	PLAYER_STATE m_nowState;
	PLAYER_STATE m_nextState;
	std::vector<class PlayerStateBase*> m_statePool;

	class SkeletalMeshComponent* m_skelComp;
	std::vector<const class Animation*> m_anims;

	// メッシュパス
	static const std::string PLAYER_MESH_PATH;
	static const std::string PLAYER_SKEL_PATH;
	// アニメーションパス
	static const std::string ANIM_IDLE_PATH;
	static const std::string ANIM_WALK_PATH;
	static const std::string ANIM_JOG_PATH;
	static const std::string ANIM_RUN_PATH;


};