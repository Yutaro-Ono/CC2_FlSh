//----------------------------------------------------------------------------------+
// @file        EnemyBase.h
// @brief       �G�l�~�[�x�[�X�N���X
// @note        Actor�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/05   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"

enum class ENEMY_STATE
{
	STATE_IDLE,
	STATE_PATROL,
	STATE_TRACK,
	STATE_ATTACK,
	STATE_DEATH
};

class EnemyBase : public Actor
{
public:


	EnemyBase(OBJECT_TAG _tag);
	~EnemyBase();

	void AddHealth(unsigned short int _addVal);
	void ReduceHealth(unsigned short int _redVal);

	ENEMY_STATE GetEnemyState() { return m_enemyState; }
	void SetEnemyState(ENEMY_STATE _state) { m_enemyState = _state; }

private:

	void SetHealth(unsigned short int _health) { m_health = _health; };     // �̗͒l�̃Z�b�^�[


protected:

	ENEMY_STATE m_enemyState;        // �G�l�~�[�̏��

	unsigned short int m_health;     // �̗͒l(0�Ŏ��S)



};