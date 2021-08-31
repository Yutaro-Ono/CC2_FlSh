#include "WorldGameScene.h"
#include "GameScene.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "Renderer.h"
#include "LevelManager.h"
#include "SkyBox.h"
#include "Math.h"
#include "PlayerCar.h"
#include "PlayerManager.h"
#include "MissionManager.h"
#include "Helicopter.h"
#include "CanvasGameScene.h"
#include "PlayerControlUI.h"
#include "PointLight.h"
#include "LandMarkIndicator.h"
#include "MiniMapHUD.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <random>

static Vector3 playerPos;
static Vector3 tempPos;

// �R���X�g���N�^
WorldGameScene::WorldGameScene()
	:m_foundPlayer(false)
{
	// �v���C���[�̐���
	m_player = new PlayerManager();
	m_player->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
	m_player->SetScale(200.0f);
	//PointLight* light = new PointLight();
	//light->SetPosition(m_player->GetPosition());
	tempPos = playerPos = m_player->GetPosition();

	// ���x���̐���
	m_level = new LevelManager(this, 0);
	// �~�b�V�����}�l�[�W���[����
	m_mission = new MissionManager(this);
	// ��(�����Ȃ�)�쐬
	m_environment = new Environment(this, Environment::GAME_TIME::NIGHT);

	// �w���R�v�^�[���O�@����
	for (int i = 0; i < 3; i++)
	{
		m_helicopters.emplace_back(new Helicopter(this, Vector3(5000.0f * (i + 1), 18000.0f * (i + 1), 1200.0f), i));
	}

	// UI����
	m_canvas = new CanvasGameScene(this);

	// �}�b�vHUD����
	m_mapHUD = new MiniMapHUD(m_player);
}

WorldGameScene::~WorldGameScene()
{
	RENDERER->RemoveMapHUD();

	for (auto heli : m_helicopters)
	{
		heli->SetState(Actor::STATE_DEAD);
	}
	m_helicopters.clear();
	m_clients.clear();
	m_patrolPoints.clear();
	delete m_player;
	delete m_canvas;
	delete m_level;
	delete m_mission;
	delete m_environment;
}

bool WorldGameScene::Load()
{
	return false;
}

void WorldGameScene::Update(float in_deltaTime)
{

	m_environment->Update();

	// �~�b�V�����̍X�V
	m_mission->Update(in_deltaTime);

	// �v���C���[�̈˗��l�T�[�`
	for (auto client : m_clients)
	{
		m_player->SearchClient(client);
	}

	// �w�����v���C���[�𔭌��������ǂ����𑖍����A��ł��q�b�g�����甲����
	for (auto heli : m_helicopters)
	{
		if (heli->GetFoundPlayer())
		{
			//m_canvas->GetPlayerControlUI()->SetFindPlayer(true);
			m_foundPlayer = true;
			break;
		}
		else
		{
			//m_canvas->GetPlayerControlUI()->SetFindPlayer(false);
			m_foundPlayer = false;
		}
	}
}

void WorldGameScene::UpdateWorld(float _deltaTime)
{
}

// �˗��l�A�N�^�̓o�^
void WorldGameScene::AddClientActor(ClientActor* in_client)
{
	m_clients.push_back(in_client);
}

// ����n�_�̓o�^
void WorldGameScene::AddPatrolPoint(PatrolPoint* in_patrolPoint)
{
	m_patrolPoints.push_back(in_patrolPoint);
}

// ����n�_�̃V���b�t��
void WorldGameScene::ShufflePatrolPoint()
{
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(m_patrolPoints.begin(), m_patrolPoints.end(), engine);
}

// �˗��l�A�N�^�z��̃V���b�t��
void WorldGameScene::ShuffleClientActor()
{
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(m_clients.begin(), m_clients.end() - 1, engine);
}

void WorldGameScene::ImGuiDebugWorld()
{
}
