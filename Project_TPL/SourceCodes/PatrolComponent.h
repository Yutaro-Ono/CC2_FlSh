#pragma once
#include "Component.h"
#include "Math.h"

class PatrolComponent : public Component
{

public:
	
	// ������
	enum PATROL_STATE
	{
		NONE,       // �������Ȃ�
		PATROL,     // �ʏ폄��
		SEARCH,     // �v���C���[�̍��G
		CHASE       // �v���C���[�̒ǔ�
	};


	PatrolComponent(class Helicopter* _owner, class WorldGameScene* _world);
	~PatrolComponent();

	void Update(float _deltaTime) override;      // �X�V����

	void MoveToPatrolPos(float _deltaTime);

	void ChasePlayer(float _deltaTime);          // �v���C���[�ǐ�

	void SetPatrolState(PatrolComponent::PATROL_STATE _state) { m_patrolState = _state; }

private:


	PATROL_STATE m_patrolState;     // ������

	class Helicopter* m_heli;
	class WorldGameScene* m_world;
	
	class PatrolPoint* m_targetPoint;     // �����̃|�C���g
	Vector3 m_targetPos;                  // ����n�_�̍��W


};