#pragma once
#include "Component.h"

class ShootComponent : public Component
{
public:

	ShootComponent(class WeaponBase* _weapon, class Player* _player);
	~ShootComponent();

	void Update(float _deltaTime) override;
	void UpdateFire(float _deltaTime);

	void ShootBullet();

	void SetFireRate(float _rate) { m_fireRate; }

private:

	class WeaponBase* m_weapon;
	class Player* m_player;


	float m_fireRate;           // 弾の発射レート
	float m_intervalCount;      // 弾発射時の間隔カウンター

};