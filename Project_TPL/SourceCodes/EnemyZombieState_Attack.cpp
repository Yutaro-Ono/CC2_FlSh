#include "EnemyZombieState_Attack.h"

EnemyZombieState_Attack::EnemyZombieState_Attack()
{
}

EnemyZombieState_Attack::~EnemyZombieState_Attack()
{
}

ZOMBIE_STATE EnemyZombieState_Attack::Update(EnemyZombie* _player, float _deltaTime)
{
	return ZOMBIE_STATE::STATE_ATTACK;
}

void EnemyZombieState_Attack::EnterState(EnemyZombie* _player)
{
}
