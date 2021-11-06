#include "DebugObjectBase.h"

class DebugWeaponAR4 : public DebugObjectBase
{

public:

	DebugWeaponAR4(class WeaponAR4* _weaponAR4);
	~DebugWeaponAR4();


	void Update(float _deltaTime) override;
	void Render() override;


private:


	class WeaponAR4* m_weaponAR4;


};