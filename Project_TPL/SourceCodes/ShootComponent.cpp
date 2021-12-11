#include "ShootComponent.h"
#include "WeaponBase.h"
#include "Player.h"
#include "GameMain.h"
#include "BulletActor.h"

ShootComponent::ShootComponent(WeaponBase* _weapon, class Player* _player)
	:Component(_weapon)
	,m_weapon(_weapon)
	,m_player(_player)
{
}

ShootComponent::~ShootComponent()
{
}

void ShootComponent::Update(float _deltaTime)
{
	UpdateShootInterval(_deltaTime);

	// �v���C���[���G�C����ԏo�Ȃ����͎ˌ��������X�L�b�v
	if (m_player->GetPlayerState() != PLAYER_STATE::STATE_AIM)
	{
		return;
	}

	// ���N���b�N�Ŕ��ˏ�����
	if (MOUSE_INSTANCE.GetLeftButtonValue(Mouse::MouseButtonState::MOUSE_BUTTON_PRESSED))
	{
		// ���ˊԊu���\���ɊJ���Ă���ꍇ�A�e����������
		if (!(m_intervalCount <= 0.0f))
		{
			ShootBullet();
		}
	}

}

/// <summary>
/// �ˌ��Ԋu�̍X�V����
/// </summary>
/// <param name="_deltaTime"></param>
void ShootComponent::UpdateShootInterval(float _deltaTime)
{
	if (m_intervalCount > 0.0f)
	{
		m_intervalCount -= 1.0f * _deltaTime;
	}

}

/// <summary>
/// �e���ˏ���
/// </summary>
void ShootComponent::ShootBullet()
{
	// �e�𐶐�������W(�e��)
	Vector3 muzzlePos = m_weapon->GetWeaponInfo().muzzlePos;

	// �y���b�g�����̒e�𐶐�����
	unsigned int pelletNum = m_weapon->GetWeaponInfo().numOfPellets;

	for (unsigned int i = 0; i < pelletNum; i++)
	{
		// �e�̔��˕����𕪎U������
		Vector3 bulletDir = m_weapon->GetForward() * 
			                Vector3(Math::Sin(20.0f * static_cast<float>(i)),
								    Math::Sin(20.0f * static_cast<float>(i)), 1.0f);

		BulletActor* bullet = new BulletActor(muzzlePos, bulletDir);
	}
	
	// ���̔��˂܂ł̃C���^�[�o�����Z�b�g
	m_intervalCount = static_cast<float>(m_weapon->GetWeaponInfo().RPS);
}
