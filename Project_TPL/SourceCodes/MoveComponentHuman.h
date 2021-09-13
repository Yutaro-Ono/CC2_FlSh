#pragma once
#include "MoveComponent.h"

class MoveComponentHuman : public MoveComponent
{

public:

	MoveComponentHuman(class Actor* in_owner);     // �R���X�g���N�^
	~MoveComponentHuman();                               // �f�X�g���N�^


	void Update(float in_deltaTime) override;                      // �R���|�[�l���g�̍X�V

	void ProcessInput(float in_deltaTime) override {}                                    // �R���|�[�l���g�̓��͏���

	void OnUpdateWorldTransform() override {}                      // ���[���h���W�ϊ��p


	void MovementByController(float in_deltaTime) override;

	void MovementByKeyboard(float in_deltaTime) override;


protected:

	bool m_toggleRun;                     // ����{�^���������ꂽ��

	float m_velocity;                     // �ړ����̉����x

	static const float PLAYER_SPEED;
	static const float SPEED_WALK;
	static const float SPEED_JOG;
	static const float SPEED_RUN;

};