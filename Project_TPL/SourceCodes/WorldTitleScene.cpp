#include "WorldTitleScene.h"
#include "CanvasTitleScene.h"
#include "Environment.h"
#include "GameMain.h"
#include "LoadScreen.h"
#include "BridgeObject.h"
#include "TitleCar.h"
#include "ClientActor.h"
#include "SkyBox.h"
#include "CubeMapComponent.h"

WorldTitleScene::WorldTitleScene()
	:m_car(nullptr)
	,m_client(nullptr)
{
}

WorldTitleScene::~WorldTitleScene()
{
	
}

bool WorldTitleScene::Load()
{


	// UI�L�����o�X�̐���
	m_canvas = new CanvasTitleScene(this);

	// �^�C�g���p�Ԃ̐���
	m_car = new TitleCar();
	m_car->SetPosition(Vector3(6500.0f, -65.0f, 0.0f));
	m_car->SetScale(0.4f);

	// �����̐���
	m_environment = new Environment(m_car, ENVIRONMENT_TYPE::NIGHT);

	// ���I�u�W�F�N�g�̐���
	for (int i = 0; i < 8; i++)
	{
		BridgeObject* bridge = new BridgeObject(1, Vector3(i * 6515.0f, -2000.0f, 0.0f));

		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}


	return true;
}

void WorldTitleScene::UpdateWorld(float _deltaTime)
{
	// �f�B���N�V���i�����C�g�̍X�V
	m_environment->SetDirectionalLightPos(m_car->GetPosition());
}