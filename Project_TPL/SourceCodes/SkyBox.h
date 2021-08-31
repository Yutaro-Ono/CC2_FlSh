#pragma once
#include "Actor.h"
#include "CubeMapComponent.h"
#include "Environment.h"

class SkyBox : public Actor
{
public:

	SkyBox(ENVIRONMENT_TYPE _envType);
	SkyBox(class Environment* _environment, ENVIRONMENT_TYPE _envType);
	~SkyBox();

	void UpdateActor(float _deltaTime) override;

	// スカイボックスのセット
	void SetSkyBox(ENVIRONMENT_TYPE _envType);

	class CubeMapComponent* GetCubeMapComp() { return m_cubeMapComp; }

private:

	class CubeMapComponent* m_cubeMapComp;

	class Environment* m_environment;

};