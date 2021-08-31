#include "Environment.h"
#include "GameMain.h"
#include "SkyBox.h"
#include "CubeMapComponent.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "RenderBloom.h"
#include "DirectionalLight.h"

static Vector3 playerPos = Vector3::Zero;
//static const Vector3 ADJUST_POS = Vector3(2000.0f, 0.0f, 3000.0f);
static const Vector3 ADJUST_POS = Vector3(-600.0f, 50.0f, 700.0f);

Environment::Environment(GAME_TIME in_gameTime, const Vector3& in_pos)
	:m_gameTime(in_gameTime)
	, m_world(nullptr)
{
	// �X�J�C�{�b�N�X����
	m_morningBox = new SkyBox(this, GAME_TIME::MORNING);
	//m_eveningBox = new SkyBox(this, GAME_TIME::EVENING);
	m_nightBox = new SkyBox(this, GAME_TIME::NIGHT);
	// �ݒ肳�ꂽ���ԑт��ƂɃX�J�C�{�b�N�X���Z�b�g
	SetSkyBox(m_gameTime);
	SetDirectionalLight(m_gameTime, in_pos);
}


Environment::Environment(GameWorld* in_world, GAME_TIME in_gameTime)
	:m_gameTime(in_gameTime)
	,m_world(in_world)
{
	// �X�J�C�{�b�N�X����
	m_morningBox = new SkyBox(this, GAME_TIME::MORNING);
	//m_eveningBox = new SkyBox(this, GAME_TIME::EVENING);
	m_nightBox = new SkyBox(this, GAME_TIME::NIGHT);

	playerPos = m_world->GetPlayer()->GetPosition();
	
	// �ݒ肳�ꂽ���ԑт��ƂɃX�J�C�{�b�N�X���Z�b�g
	SetSkyBox(m_gameTime);

	// �����Z�b�g
	SetDirectionalLight(in_gameTime, m_world->GetPlayer()->GetPosition());

}

Environment::~Environment()
{

}

void Environment::Update()
{

	// ���t���[���̃v���C���[���W
	Vector3 framePlayerPos = m_world->GetPlayer()->GetPosition();

	// �e�̂������}���邽��4000�ȏ�̈ړ��ŕ��s���C�g�̈ʒu���v���C���[�ɃX�i�b�v����
	if (framePlayerPos.x >= playerPos.x + 2000 || framePlayerPos.x <= playerPos.x - 1000 ||
		framePlayerPos.y >= playerPos.y + 2000 || framePlayerPos.y <= playerPos.y - 2000)
	{
		// �v���C���[�̈ʒu�ɉ����ĕ��s���C�g�̍��W�E�������œK��
		RENDERER->GetDirectionalLight()->SetPosition(Vector3((int)framePlayerPos.x + (int)ADJUST_POS.x, 
			                                                 (int)framePlayerPos.y + (int)ADJUST_POS.y, 
			                                                  0.0f + (int)ADJUST_POS.z));

		RENDERER->GetDirectionalLight()->SetTarget(framePlayerPos);

		playerPos = framePlayerPos;
	}

}

void Environment::SetSkyBox(GAME_TIME in_gameTime)
{
	if (in_gameTime == GAME_TIME::MORNING || in_gameTime == GAME_TIME::AFTERNOON)
	{
		// �����_���[�ɓ����̃X�J�C�{�b�N�X���Z�b�g
		RENDERER->SetActiveSkyBox(m_morningBox->GetCubeMapComp());
		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (in_gameTime == GAME_TIME::EVENING)
	{
		// �����_���[�ɗ[���̃X�J�C�{�b�N�X���Z�b�g
		RENDERER->SetActiveSkyBox(m_eveningBox->GetCubeMapComp());
		m_eveningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (in_gameTime == GAME_TIME::NIGHT)
	{
		// �����_���[�ɖ�̃X�J�C�{�b�N�X���Z�b�g
		RENDERER->SetActiveSkyBox(m_nightBox->GetCubeMapComp());
		m_nightBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

}

// ���ԑт��Ƃɕ��s���C�g��ݒ肷��
void Environment::SetDirectionalLight(GAME_TIME in_gameTime, const Vector3& in_pos)
{
	m_gameTime = in_gameTime;

	if (m_gameTime == GAME_TIME::MORNING)
	{
		// ���C�e�B���O
		RENDERER->SetAmbientLight(Vector3(0.5f, 0.5f, 0.52f));

		// �f�B���N�V���i�����C�g�̒���
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.5f, 0.5f, 0.51f));
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.4f, 0.5f, 0.5f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.3f, 0.3f, 0.3f));
		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(0.085f);
		RENDERER->GetBloom()->SetExposureVal(4.5f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (m_gameTime == GAME_TIME::AFTERNOON)
	{
		// ���C�e�B���O
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));

		// �f�B���N�V���i�����C�g�̒���
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.7f, 0.7f, 0.75f));
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.4f, 0.4f, 0.45f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.5f, 0.5f, 0.6f));
		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(1.0f);
		RENDERER->GetBloom()->SetExposureVal(4.5f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (m_gameTime == GAME_TIME::EVENING)
	{
		// ���C�e�B���O
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));

		// �f�B���N�V���i�����C�g�̒���
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.4f, 0.4f, 0.4f));
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.07f, 0.05f, 0.3f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.01f, 0.0f, 0.1f));
		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(1.0f);
		RENDERER->GetBloom()->SetExposureVal(4.5f);

		m_eveningBox->GetCubeMapComp()->SetIsVisible(true);
		return;

	}

	if (m_gameTime == GAME_TIME::NIGHT)
	{
		// �f�B���N�V���i�����C�g�̒���
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.1f, 0.485f, 0.46f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.1f, 0.35f, 0.4f));
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.31f, 0.415f, 0.46f));
		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(0.085f);
		RENDERER->GetBloom()->SetExposureVal(5.5f);
		// ��p�X�J�C�{�b�N�X�̗L����
		m_nightBox->GetCubeMapComp()->SetIsVisible(true);

		return;
	}

}

// �f�B���N�V���i�����C�g�̌����ƌ������Čv�Z
void Environment::SetDirectionalLightPos(const Vector3& in_pos)
{
	// �v���C���[�̈ʒu�ɉ����ĕ��s���C�g�̍��W�E�������œK��
	RENDERER->GetDirectionalLight()->SetPosition(Vector3(in_pos.x + (int)ADJUST_POS.x, 
		                                                 in_pos.y + (int)ADJUST_POS.y, 
		                                                 in_pos.z + (int)ADJUST_POS.z));

	RENDERER->GetDirectionalLight()->SetTarget(in_pos);
}
