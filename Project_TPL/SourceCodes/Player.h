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
	STATE_AIM,

	STATE_JUMP_START,
	STATE_JUMP_FALL,
	STATE_JUMP_LAND,

	STATE_WEAPONOUT_IDLE,
	STATE_WEAPONOUT_MOVEFWD,
	STATE_WEAPONOUT_MOVEBWD,
	STATE_WEAPONOUT_MOVERIGHT,
	STATE_WEAPONOUT_MOVELEFT,
	STATE_WEAPONOUT_WALKFWD,
	STATE_WEAPONOUT_WALKBWD,
	STATE_WEAPONOUT_WALKRIGHT,
	STATE_WEAPONOUT_WALKLEFT,
	STATE_WEAPONOUT_SPRINT,

	STATE_AIM_STAND,
	STATE_AIM_CROUCH,
	STATE_AIM_DOWN_STAND,
	STATE_AIM_DOWN_CROUCH,

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
	void UpdateWeaponOut();


	bool GetToggleSprint() { return m_toggleSprint; }
	bool GetToggleWalk() { return m_toggleWalk; }
	bool GetToggleCrouch() { return m_toggleCrouch; }
	bool GetToggleWeaponOut() { return m_toggleWeaponOut; }
	PLAYER_STATE GetPlayerState() { return m_nowState; }

	void SetToggleSprint(bool _toggle) { m_toggleSprint = _toggle; }
	void SetToggleWalk(bool _toggle) { m_toggleWalk   = _toggle; }
	void SetToggleCrouch(bool _toggle) { m_toggleCrouch = _toggle; }
	void SetToggleWeaponOut(bool _toggle) { m_toggleWeaponOut = _toggle; }

	void SetActiveAimCamera(bool _isAim);
	class FirstPersonCameraComponent* GetFPSCamera() { return m_fpsCamera; }

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
	bool m_toggleWeaponOut;
	bool m_isWeaponOutChange;
	unsigned int m_weaponOutPressStart;
	unsigned int m_weaponOutPressCount;

	std::vector<const class Animation*> m_anims;

	// カメラ
	class TPSCamera* m_tpsCamera;                       // 三人称視点カメラ
	class FirstPersonCameraComponent* m_fpsCamera;      // 一人称視点カメラ

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

	static const std::string ANIM_WEAPOUT_IDLE_PATH;
	static const std::string ANIM_WEAPOUT_MOVE_FWD_PATH;
	static const std::string ANIM_WEAPOUT_MOVE_BWD_PATH;
	static const std::string ANIM_WEAPOUT_MOVE_RIGHT_PATH;
	static const std::string ANIM_WEAPOUT_MOVE_LEFT_PATH;
	static const std::string ANIM_WEAPOUT_WALK_FWD_PATH;
	static const std::string ANIM_WEAPOUT_WALK_BWD_PATH;
	static const std::string ANIM_WEAPOUT_WALK_RIGHT_PATH;
	static const std::string ANIM_WEAPOUT_WALK_LEFT_PATH;

	static const std::string ANIM_AIM_STAND;
	static const std::string ANIM_AIM_CROUCH;
};