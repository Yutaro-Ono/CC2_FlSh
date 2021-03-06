//----------------------------------------------------------------------------------+
// @file        MotorBikeParent.h
// @brief       バイク(乗り物)親クラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/ 6   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Vehicle.h"
#include <vector>


class MotorBikeParent : public Vehicle
{

public:

	enum class MOTORBIKE_PARTS
	{
		BODY = 0,
		HANDLE,
		FRONT_WHEEL,
		BACK_WHEEL,
		ALL
	};

	MotorBikeParent();
	~MotorBikeParent();

	void UpdateActor(float _deltaTime) override;


private:

	// 各パーツをアクターで定義
	class Actor* m_parts[static_cast<int>(MOTORBIKE_PARTS::ALL)];

	// 各パーツの座標修正用ベクトル
	Vector3 m_offsetPos[static_cast<int>(MOTORBIKE_PARTS::ALL)];

	// メッシュファイルパス
	static const std::string STR_MOTORBIKE_BODY;
	static const std::string STR_MOTORBIKE_HANDLE;
	static const std::string STR_MOTORBIKE_WHEEL;

};