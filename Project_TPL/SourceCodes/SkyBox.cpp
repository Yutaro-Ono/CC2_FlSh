#include "SkyBox.h"
#include "CubeMapComponent.h"
#include "GameMain.h"
#include <string>


SkyBox::SkyBox(ENVIRONMENT_TYPE _envType)
	:Actor(OBJECT_TAG::SKYBOX)
	,m_environment(nullptr)
{
	SetSkyBox(_envType);
	m_position = Vector3(100, 100, 100);
}

SkyBox::SkyBox(Environment* _environment, ENVIRONMENT_TYPE _envType)
	:Actor(OBJECT_TAG::SKYBOX)
	,m_environment(_environment)
{
	SetSkyBox(_envType);
	m_position = Vector3(100, 100, 100);
}

SkyBox::~SkyBox()
{
	RENDERER->RemoveActiveSkyBox(m_cubeMapComp);
}

void SkyBox::UpdateActor(float _deltaTime)
{

}

// 時間帯ごとにスカイボックスを生成
void SkyBox::SetSkyBox(ENVIRONMENT_TYPE _envType)
{
	if (_envType == ENVIRONMENT_TYPE::MORNING)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/Skybox/morning/");
		m_cubeMapComp->SetIntensity(0.5f);
		return;
	}
	if (_envType == ENVIRONMENT_TYPE::AFTERNOON)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/Skybox/morning/");
		m_cubeMapComp->SetIntensity(0.65f);
		return;
	}
	if (_envType == ENVIRONMENT_TYPE::EVENING)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/Skybox/sunset/");
		m_cubeMapComp->SetIntensity(0.45f);
		return;
	}
	if (_envType == ENVIRONMENT_TYPE::NIGHT)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/Skybox/NightCity/");
		m_cubeMapComp->SetIntensity(0.5f);
		return;
	}

	if (_envType == ENVIRONMENT_TYPE::SPACE)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/Skybox/NightCity/");
		m_cubeMapComp->SetIntensity(0.5f);
		return;
	}

	if (_envType == ENVIRONMENT_TYPE::TITLE)
	{
		m_cubeMapComp = new CubeMapComponent(this);
		m_cubeMapComp->CreateTexture("Data/Textures/Skybox/NightCity/");
		m_cubeMapComp->SetIntensity(0.5f);
		return;
	}
}
