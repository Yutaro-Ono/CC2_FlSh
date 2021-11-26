#include "GameSceneTPL.h"
#include "GameMain.h"
#include "LoadScreen.h"
#include "WorldGameSceneTPL.h"
#include "CanvasGameSceneTPL.h"
#include "Environment.h"
#include "ClientActor.h"
#include "MotorBikeParent.h"
#include "Player.h"
#include "EnemyZombie.h"
#include "WeaponAR4.h"
#include "PlayerHUD.h"
#include "MiniMapHUD.h"
#include "AudioManager.h"
#include "ParticleManager.h"

#include "MeshGpmesh.h"
#include "LevelBlock.h"

GameSceneTPL::GameSceneTPL()
	:m_sceneState(SCENE_STATE::FADE_IN)
	,m_canvas(nullptr)
{
}

GameSceneTPL::~GameSceneTPL()
{
	delete m_world;
	delete m_canvas;
	// 全てのアクターを削除
	GAME_INSTANCE.DeadAllActor();
	// 全てのUIをCloseに設定
	for (auto iter : GAME_INSTANCE.GetUIStack())
	{
		iter->Close();
	}
	// パーティクルを全て削除
	RENDERER->GetParticleManager()->AllDeadParticle();
	// 音楽を停止
	AUDIO->StopMusic();
}

void GameSceneTPL::Initialize()
{

	// ロード画面の有効化
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
		GAME_INSTANCE.GetLoadScreen()->SetIsGame();
	}


	// ゲームシーン用ワールドの生成
	m_world = new WorldGameSceneTPL(this);
	m_world->Load();

	// UIキャンバスの生成
	m_canvas = new CanvasGameSceneTPL(m_world, this);
	m_canvas->Load();


	// ロード演出(残り分)
	for (int i = 0; i < GAME_INSTANCE.GetLoadScreen()->GetGaugeAllNum(); i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}


	// ロード画面の無効化
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
}

SceneBase* GameSceneTPL::Update(float _deltaTime)
{
	return this;
}

void GameSceneTPL::Draw()
{
}
