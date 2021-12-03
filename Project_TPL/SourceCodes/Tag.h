//----------------------------------------------------------------------------------+
// @file        Tag.h
// @brief       �^�O�N���X
// @note        �Q�[�����v�f����ޕ�������
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/28   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include <stdexcept>
#include <iterator>

enum class OBJECT_TAG : unsigned char
{
	BEGIN,

	SYSTEM = BEGIN,

	CAMERA,

	LIGHT,

	ACTOR_PLAYER,
	ACTOR_NPC,
	ACTOR_ENEMY,
	ACTOR_VEHICLE,
	ACTOR_WEAPON,

	STATIC_OBJECT,

	ATTACK_ENEMY,
	ATTACK_BULLET,


	SKYBOX,
	PARTICLE,

	DEBUG_SPHERE,

	END
};


// �C���N�������g (�S�Ẵ^�O�O���[�v�̃��[�v���\)
OBJECT_TAG& operator++(OBJECT_TAG& rhs);