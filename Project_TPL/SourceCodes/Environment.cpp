#include "Environment.h"
#include "GameMain.h"
#include "SkyBox.h"
#include "CubeMapComponent.h"
#include "WorldGameScene.h"
#include "PlayerManager.h"
#include "RenderBloom.h"
#include "DirectionalLight.h"

static Vector3 playerPos = Vector3::Zero;
//static const Vector3 ADJUST_POS = Vector3(2000.0f, 0.0f, 3000.0f);
static const Vector3 ADJUST_POS = Vector3(-600.0f, 50.0f, 700.0f);


Environment::Environment(Actor* _target, ENVIRONMENT_TYPE _envType)
	:m_envType(_envType)
	,m_lightTarget(_target)
{
	// �X�J�C�{�b�N�X����
	m_skyBox = new SkyBox(this, m_envType);

	playerPos = m_lightTarget->GetPosition();
	
	// �ݒ肳�ꂽ���ԑт��ƂɃX�J�C�{�b�N�X���Z�b�g
	SetSkyBox(m_skyBox);

	// �����Z�b�g
	SetDirectionalLight(_envType, m_lightTarget->GetPosition());

}

Environment::~Environment()
{

}

void Environment::Update()
{

	// ���t���[���̃v���C���[���W
	Vector3 framePlayerPos = m_lightTarget->GetPosition();

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

void Environment::SetSkyBox(class SkyBox* _skybox)
{
	RENDERER->SetActiveSkyBox(m_skyBox->GetCubeMapComp());
	m_skyBox->GetCubeMapComp()->SetIsVisible(true);
}

// ���ԑт��Ƃɕ��s���C�g��ݒ肷��
void Environment::SetDirectionalLight(ENVIRONMENT_TYPE _envType, const Vector3& _pos)
{
	m_envType = _envType;

	if (m_envType == ENVIRONMENT_TYPE::MORNING)
	{
		// ���C�e�B���O
		RENDERER->SetAmbientLight(Vector3(0.5f, 0.5f, 0.52f));

		// �f�B���N�V���i�����C�g�̒���
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.741f, 0.735f, 0.735f));
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.872f, 0.725f, 0.751f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.901f, 0.741f, 0.781f));
		RENDERER->GetDirectionalLight()->SetIntensity(0.3f);
		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	if (m_envType == ENVIRONMENT_TYPE::AFTERNOON)
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
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	if (m_envType == ENVIRONMENT_TYPE::EVENING)
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
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	if (m_envType == ENVIRONMENT_TYPE::NIGHT)
	{
		// �f�B���N�V���i�����C�g�̒���
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.1f, 0.485f, 0.46f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.1f, 0.35f, 0.4f));
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.31f, 0.415f, 0.46f));
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	if (m_envType == ENVIRONMENT_TYPE::NIGHT_CITY)
	{
		// �f�B���N�V���i�����C�g�̒���
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.1f, 0.485f, 0.46f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.1f, 0.35f, 0.4f));
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.31f, 0.415f, 0.46f));
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}


	if (m_envType == ENVIRONMENT_TYPE::SPACE)
	{
		// �f�B���N�V���i�����C�g�̒���
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.1f, 0.485f, 0.46f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.1f, 0.35f, 0.4f));
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.31f, 0.415f, 0.46f));
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// �u���[���̐ݒ�
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	// �X�J�C�{�b�N�X�̗L����
	m_skyBox->GetCubeMapComp()->SetIsVisible(true);

}

// �f�B���N�V���i�����C�g�̌����ƌ������Čv�Z
void Environment::SetDirectionalLightPos(const Vector3& _pos)
{
	// �v���C���[�̈ʒu�ɉ����ĕ��s���C�g�̍��W�E�������œK��
	RENDERER->GetDirectionalLight()->SetPosition(Vector3(_pos.x + (int)ADJUST_POS.x, 
		                                                 _pos.y + (int)ADJUST_POS.y, 
		                                                 _pos.z + (int)ADJUST_POS.z));

	RENDERER->GetDirectionalLight()->SetTarget(_pos);
}
