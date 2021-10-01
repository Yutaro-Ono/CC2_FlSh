#include "EnemyZombieState_Run.h"

EnemyZombieState_Run::EnemyZombieState_Run()
{
}

EnemyZombieState_Run::~EnemyZombieState_Run()
{
}

ZOMBIE_STATE EnemyZombieState_Run::Update(EnemyZombie* _player, float _deltaTime)
{
	return ZOMBIE_STATE::STATE_RUN;
}

void EnemyZombieState_Run::EnterState(EnemyZombie* _player, float _deltaTime)
{
}
