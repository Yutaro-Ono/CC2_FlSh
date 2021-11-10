#pragma once
#include "Actor.h"


class LandMarkIndicator : public Actor
{

public:

	LandMarkIndicator(class Actor* in_player);
	~LandMarkIndicator();

	void UpdateActor(float in_deltaTime) override;

	void SetTargetActor(class Actor* _target);

private:

	class Actor* m_target;

};
