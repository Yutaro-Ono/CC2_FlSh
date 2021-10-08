#include "EnemyZombieState_Death.h"

EnemyZombieState_Death::EnemyZombieState_Death()
{
}

EnemyZombieState_Death::~EnemyZombieState_Death()
{
}

ZOMBIE_STATE EnemyZombieState_Death::Update(EnemyZombie* _player, float _deltaTime)
{
	return ZOMBIE_STATE::STATE_DEATH;
}

void EnemyZombieState_Death::EnterState(EnemyZombie* _player)
{
}
