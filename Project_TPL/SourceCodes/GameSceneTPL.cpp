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
	// �S�ẴA�N�^�[���폜
	GAME_INSTANCE.DeadAllActor();
	// �S�Ă�UI��Close�ɐݒ�
	for (auto iter : GAME_INSTANCE.GetUIStack())
	{
		iter->Close();
	}
	// �p�[�e�B�N����S�č폜
	RENDERER->GetParticleManager()->AllDeadParticle();
	// ���y���~
	AUDIO->StopMusic();
}

void GameSceneTPL::Initialize()
{

	// ���[�h��ʂ̗L����
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
		GAME_INSTANCE.GetLoadScreen()->SetIsGame();
	}


	// �Q�[���V�[���p���[���h�̐���
	m_world = new WorldGameSceneTPL(this);
	m_world->Load();

	// UI�L�����o�X�̐���
	m_canvas = new CanvasGameSceneTPL(m_world, this);
	m_canvas->Load();


	// ���[�h���o(�c�蕪)
	for (int i = 0; i < GAME_INSTANCE.GetLoadScreen()->GetGaugeAllNum(); i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}


	// ���[�h��ʂ̖�����
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
}

SceneBase* GameSceneTPL::Update(float _deltaTime)
{
	return this;
}

void GameSceneTPL::Draw()
{
}
