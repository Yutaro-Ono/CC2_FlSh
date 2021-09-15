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
	STATE_SPRINT,

	STATE_CROUCH,
	STATE_CROUCH_MOVE,

	STATE_JUMP_START,
	STATE_JUMP_FALL,
	STATE_JUMP_LAND,

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

	bool GetToggleSprint() { return m_toggleSprint; }
	bool GetToggleWalk() { return m_toggleWalk; }
	bool GetToggleCrouch() { return m_toggleCrouch; }
	void SetToggleSprint(bool _toggle) { m_toggleSprint = _toggle; }
	void SetToggleWalk(bool _toggle) { m_toggleWalk   = _toggle; }
	void SetToggleCrouch(bool _toggle) { m_toggleCrouch = _toggle; }

private:

	void UpdatePlayerState(float _deltaTime);


	// ステート制御用
	PLAYER_STATE m_nowState;
	PLAYER_STATE m_nextState;
	std::vector<class PlayerStateBase*> m_statePool;

	// 移動状態のトグル関連
	bool m_toggleSprint;
	bool m_toggleWalk;
	bool m_toggleCrouch;

	class SkeletalMeshComponent* m_skelComp;
	std::vector<const class Animation*> m_anims;

	// カメラ
	class TPSCamera* m_tpsCamera;

	// 追従するポイントライト
	class PointLight* m_light;


	// メッシュパス
	static const std::string PLAYER_MESH_PATH;
	static const std::string PLAYER_SKEL_PATH;
	// アニメーションパス
	static const std::string ANIM_IDLE_PATH;
	static const std::string ANIM_WALK_PATH;
	static const std::string ANIM_JOG_PATH;
	static const std::string ANIM_RUN_PATH;
	static const std::string ANIM_CROUCH_PATH;
	static const std::string ANIM_CROUCH_MOVE_PATH;
	static const std::string ANIM_JUMP_START_PATH;
	static const std::string ANIM_JUMP_FALL_PATH;
	static const std::string ANIM_JUMP_LAND_PATH;


};