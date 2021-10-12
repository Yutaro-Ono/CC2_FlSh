//----------------------------------------------------------------------------------+
// @file        WeaponAR4.h
// @brief       武器アクター：AR4
// @note        AR4の情報を持つ
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/12   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "WeaponBase.h"

class WeaponAR4 : public WeaponBase
{

public:

	WeaponAR4();
	WeaponAR4(class Actor* _owner);
	~WeaponAR4();



private:

	void Initialize();


protected:




	static const std::string AR4_MESH_PATH;

};