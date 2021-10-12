//----------------------------------------------------------------------------------+
// @file        WeaponBase.h
// @brief       武器ベースクラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/12   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class WeaponBase : public Actor
{

public:

	WeaponBase();
	~WeaponBase();

	void UpdateActor(float _deltaTime) override;

	void SetSocketNum(size_t _socketNum) { m_socketNum = _socketNum; }

protected:

	void SetOwnerActor(class Actor* _owner);
	void RemoveOwnerActor();

	void SetSocketMat(Matrix4 _socketMat);



	class Actor* m_owner;              // このクラスを所有するアクターポインタ
	bool m_existsOwner;                // この武器クラスを所有するオーナーアクタ―は存在するか

	Matrix4 m_socketMat;               // この武器オブジェクトをスナップするオーナーのソケット行列
	size_t m_socketNum;                 // ボーンのソケット番号

	unsigned int m_attackInterval;     // 攻撃の間隔(ミリ秒単位)
	unsigned int m_attackStartCount;   // 攻撃開始時のカウント
	unsigned int m_attackNowCount;     // 攻撃カウント更新用変数(attackStartCountと比較)

	// 弾丸関係変数
	unsigned int m_ammoCount;          // 現在のマガジンに入ってる弾薬量
	unsigned int m_ammoBullet;         // 1マガジンに入る弾薬量
	unsigned int m_ammoMax;            // 所持できる最大弾薬量
	bool m_isMelee;                    // 近接武器かどうか


};