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

	// プレイヤーがエイム状態出ない時は射撃処理をスキップ
	if (m_player->GetPlayerState() != PLAYER_STATE::STATE_AIM)
	{
		return;
	}

	// 左クリックで発射処理へ
	if (MOUSE_INSTANCE.GetLeftButtonValue(Mouse::MouseButtonState::MOUSE_BUTTON_PRESSED))
	{
		// 発射間隔が十分に開いている場合、弾生成処理へ
		if (!(m_intervalCount <= 0.0f))
		{
			ShootBullet();
		}
	}

}

/// <summary>
/// 射撃間隔の更新処理
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
/// 弾発射処理
/// </summary>
void ShootComponent::ShootBullet()
{
	// 弾を生成する座標(銃口)
	Vector3 muzzlePos = m_weapon->GetWeaponInfo().muzzlePos;

	// ペレット数分の弾を生成する
	unsigned int pelletNum = m_weapon->GetWeaponInfo().numOfPellets;

	for (unsigned int i = 0; i < pelletNum; i++)
	{
		// 弾の発射方向を分散させる
		Vector3 bulletDir = m_weapon->GetForward() * 
			                Vector3(Math::Sin(20.0f * static_cast<float>(i)),
								    Math::Sin(20.0f * static_cast<float>(i)), 1.0f);

		BulletActor* bullet = new BulletActor(muzzlePos, bulletDir);
	}
	
	// 次の発射までのインターバルをセット
	m_intervalCount = static_cast<float>(m_weapon->GetWeaponInfo().RPS);
}
