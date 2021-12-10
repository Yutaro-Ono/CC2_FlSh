#pragma once
#include "Component.h"

class ShootComponent : public Component
{
public:

	ShootComponent(class WeaponBase* _weapon);
	~ShootComponent();

	void Update(float _deltaTime) override;


	void SetFireRate(float _rate) { m_fireRate; }

private:

	class WeaponBase* m_weapon;
	


	float m_fireRate;           // 弾の発射レート
	float m_intervalCount;      // 弾発射時の間隔カウンター

};