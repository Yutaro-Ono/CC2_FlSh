#include "EnemyZombieState_Walk.h"

EnemyZombieState_Walk::EnemyZombieState_Walk()
{
}

EnemyZombieState_Walk::~EnemyZombieState_Walk()
{
}

ZOMBIE_STATE EnemyZombieState_Walk::Update(EnemyZombie* _player, float _deltaTime)
{
	return ZOMBIE_STATE::STATE_WALK;
}

void EnemyZombieState_Walk::EnterState(EnemyZombie* _player, float _deltaTime)
{
}
