#pragma once
#include "Actor.h"


class CameraActor : public Actor
{

public:

	CameraActor();
	~CameraActor();

	void UpdateActor(float _deltaTime) override;

private:


	Matrix4 m_view, m_projection;

	float m_fov;

	int m_near, m_far;

};