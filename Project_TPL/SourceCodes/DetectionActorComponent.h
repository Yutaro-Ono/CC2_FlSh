//----------------------------------------------------------------------------------+
// @file        DetectionActorComponent.h
// @brief       �A�N�^�[�����o����R���|�[�l���g
// @note        �~�`�̍��G�͈͂ɐN�������������o
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 11/05   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Component.h"


class DetectionActorComponent : public Component
{

public:

	DetectionActorComponent(class EnemyBase* _owner, class Actor* _target);
	~DetectionActorComponent();

	void Update(float _deltaTime) override;

	void SetTarget(class Actor* _target);

	bool GetIsDetected() { return m_isDetected; }

private:

	void DetectionActor();

	class EnemyBase* m_enemyOwner;    // �I�[�i�[�G�l�~�[
	class Actor* m_target;            // ���o�ΏۃA�N�^�[

	class Sphere* m_detectionArea;    // ���o�͈�(�~�`)
	float m_detectionRadius;          // ���o�͈͂̔��a


	bool m_isDetected;                // ���o�ł������ǂ���
};