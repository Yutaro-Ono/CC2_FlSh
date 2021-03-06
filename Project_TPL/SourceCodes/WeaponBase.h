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

// 武器情報構造体
typedef struct WeaponInfo
{
	Vector3 muzzlePos;             // 銃口位置(発射時の弾の始点)
	unsigned int RPM;              // 1分間の連射速度(Rate Per Minute)
	unsigned int RPS;              // 1秒間の連射速度(Rate Per Second)
	float effectiveRange;          // 有効射程
	float initialVelocity;         // 発射した弾の初速
	unsigned int numOfPellets;     // ペレット数
}WeaponInfo;

class WeaponBase : public Actor
{

public:

	WeaponBase();
	~WeaponBase();

	void UpdateActor(float _deltaTime) override;

	virtual void AdjustWorldMatToOwnerBone(const Matrix4& _boneLocalMat, float _deltaTime) = 0;

	void SetSocketNum(size_t _socketNum) { m_socketNum = _socketNum; }

	void SetOwnerActor(class Actor* _owner);
	void RemoveOwnerActor();
	void SetPlayer(class Player* _player);
	void RemovePlayer();

	class Player* GetOwnerPlayer() { return m_ownerPlayer; }
	bool GetExistsOwner() { return m_existsOwner; }

	void SetWeaponInfo(WeaponInfo& _weapInfo);
	const WeaponInfo& GetWeaponInfo() { return m_weaponInfo; }

protected:

	void InitWeaponInfo();
	void SetWeaponInfo(const Vector3& _muzzlePos, unsigned int _rpm, unsigned int _rps,
		               float _effectRange, float initVelocity, unsigned int _numofPellets);

	void SetSocketMat(Matrix4 _socketMat);


	WeaponInfo m_weaponInfo;                             // 武器情報(連射速度、有効射程等)

	class Actor* m_owner;                                // このクラスを所有するアクターポインタ
	bool m_existsOwner;                                  // この武器クラスを所有するオーナーアクタ―は存在するか

	class Player* m_ownerPlayer;                         // このクラスを所有するプレイヤーポインタ
	class AttachWeaponToBoneComponent* m_attachComp;     // 武器のボーンアタッチ用コンポーネント

	Matrix4 m_socketMat;                                 // この武器オブジェクトをスナップするオーナーのソケット行列
	size_t m_socketNum;                                  // ボーンのソケット番号

	unsigned int m_attackInterval;     // 攻撃の間隔(ミリ秒単位)
	unsigned int m_attackStartCount;   // 攻撃開始時のカウント
	unsigned int m_attackNowCount;     // 攻撃カウント更新用変数(attackStartCountと比較)

	// 弾丸関係変数
	unsigned int m_ammoCount;          // 現在のマガジンに入ってる弾薬量
	unsigned int m_ammoBullet;         // 1マガジンに入る弾薬量
	unsigned int m_ammoMax;            // 所持できる最大弾薬量
	bool m_isMelee;                    // 近接武器かどうか

};