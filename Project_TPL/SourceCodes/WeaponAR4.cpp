#include "WeaponAR4.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "AttachWeaponToBoneComponent.h"
#include "Player.h"
#include "FirstPersonCameraComponent.h"

// オーナーアクターのボーンに武器をスナップする場合のボーン番号
const unsigned int WeaponAR4::SOCKET_NUM_SPINE = 1;          // 背骨
const unsigned int WeaponAR4::SOCKET_NUM_RIGHTHAND = 33;     // 右手
const unsigned int WeaponAR4::SOCKET_NUM_LEFTHAND = 10;      // 左手

// ボーンスナップ時の座標調整用ベクトル
const Vector3 WeaponAR4::ADJUST_POS_BASIC = Vector3(-14.0f, -105.0f, 29.0f);
const Vector3 WeaponAR4::ADJUST_POS_IDLE_WEAPONOUT = Vector3(-115.0f, 23.0f, 29.0f);
const Vector3 WeaponAR4::ADJUST_POS_MOVE_WEAPONOUT = Vector3(-57.0f, -15.0f, -118.0f);

const std::string WeaponAR4::AR4_MESH_PATH = "Data/Meshes/Weapons/AR4/SM_AR4.gpmesh";

WeaponAR4::WeaponAR4()
	:m_fpsCamera(nullptr)
{
	Initialize();
}

WeaponAR4::WeaponAR4(Actor* _owner)
	:m_fpsCamera(nullptr)
{
	// オーナーアクターをセット
	m_existsOwner = true;
	m_owner = _owner;

	Initialize();
}

WeaponAR4::WeaponAR4(Player* _owner)
	:m_fpsCamera(_owner->GetFPSCamera())
{
	// オーナーアクターをセット
	m_existsOwner = true;
	m_owner = _owner;
	m_ownerPlayer = _owner;

	Initialize();
}

WeaponAR4::~WeaponAR4()
{
}

void WeaponAR4::Initialize()
{
	// AR4のメッシュをロード
	Mesh* mesh = RENDERER->GetMesh(AR4_MESH_PATH);
	
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// アタッチコンポーネント
	m_attachComp = new AttachWeaponToBoneComponent(this, m_ownerPlayer->GetSkelComp(), SOCKET_NUM_SPINE);

	//m_position = Vector3(0.0f, 10.0f, 103.0f);
	m_position = Vector3(3.0f, 0.0f, 102.5f);
	m_scale = Vector3(0.85f, 0.85f, 0.85f);
}

/// <summary>
/// 武器モデルのワールド変換行列をオーナーアクターのボーンに合わせる
/// コンポーネント側でのみこの関数を呼び出す
/// </summary>
/// <param name="_localBoneMat"> アタッチするボーンのモデル行列 * オーナーアクターのワールド変換行列 </param>
/// <param name="_deltaTime"> デルタタイム </param>
void WeaponAR4::AdjustWorldMatToOwnerBone(const Matrix4& _boneWorldMat, float _deltaTime)
{
	// プレイヤーが武器を出している場合と出していない場合
    // アタッチするソケットを変更
	if (m_ownerPlayer->GetToggleWeaponOut())
	{
		// エイム状態
		if (m_ownerPlayer->GetPlayerState() == PLAYER_STATE::STATE_AIM)
		{
			// アタッチするボーンの指定と微調整用座標の更新
			m_attachComp->ChangeAttachBoneNum(10, Vector3::Zero);
			//m_socketNum = 10;
			//m_attachPos = Vector3::Zero;

			SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));

			if (m_fpsCamera != nullptr)
			{
				Vector3 forwardVec = m_fpsCamera->GetCameraForwardVec();
				//forwardVec.z = 0.0f;
				RotateToNewForward(forwardVec);
			}

			m_recomputeWorldTransform = false;
			//m_worldTransform = Matrix4::CreateScale(m_scale)
			//	//* Matrix4::CreateTranslation(Vector3(-34.0f, 25.0f, -117.0f))
			//	//* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(60.0f)))
			//	//* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(-180.0f)))
			//	* Matrix4::CreateFromQuaternion(m_rotation)
			//	* m_socketMat
			//	* m_owner->GetWorldTransform();

			float yaw = m_fpsCamera->GetYaw();
			float pitch = m_fpsCamera->GetPitch();
			Vector3 rotatePos;
			rotatePos.x = cosf(pitch) * cosf(yaw);
			rotatePos.y = cosf(pitch) * sinf(yaw);
			rotatePos.z = sinf(pitch);



			//m_worldTransform =
			//	Matrix4::CreateRotationY(Math::ToRadians(180.0f))
			//	* Matrix4::CreateRotationX(rotatePos.z)
			//	* Matrix4::CreateRotationZ(Math::ToRadians(-45.0f))
			//	* Matrix4::CreateTranslation(m_position)
			//	//Matrix4::CreateRotationY(rotatePos.z)
			//	//* Matrix4::CreateRotationZ(rotatePos.y)
			//	//* m_ownerPlayer->GetWorldTransform();
			//	* _boneWorldMat;
			
			m_worldTransform =
				 Matrix4::CreateRotationX(-rotatePos.z)
				* Matrix4::CreateRotationZ(Math::ToRadians(-90.0f))
				* Matrix4::CreateTranslation(m_position)
				//Matrix4::CreateRotationY(rotatePos.z)
				//* Matrix4::CreateRotationZ(rotatePos.y)
				//* m_ownerPlayer->GetWorldTransform();
				* m_ownerPlayer->GetWorldTransform();

			return;
		}


		// 待機状態と移動状態ではスナップするソケットを変更
		// 移動状態
		if (m_ownerPlayer->GetPlayerState() >= PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD &&
			m_ownerPlayer->GetPlayerState() <= PLAYER_STATE::STATE_WEAPONOUT_SPRINT)
		{
			// アタッチするボーンの指定と微調整用座標の更新
			m_attachComp->ChangeAttachBoneNum(10, ADJUST_POS_MOVE_WEAPONOUT);
			m_attachComp->SetAdjustAngles(Vector3(-80.0f, -180.0f, 15.0f));
			m_socketNum = 10;

			SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
			m_recomputeWorldTransform = false;
			m_worldTransform = Matrix4::CreateScale(m_scale)
				* Matrix4::CreateTranslation(ADJUST_POS_MOVE_WEAPONOUT)
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(15.0f)))
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(-180.0f)))
				//* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(-80.0f)))
				* _boneWorldMat;

			return;
		}
		// 待機状態
		else if (m_ownerPlayer->GetPlayerState() == PLAYER_STATE::STATE_WEAPONOUT_IDLE)
		{
			// アタッチするボーンの指定と微調整用座標の更新
			m_attachComp->ChangeAttachBoneNum(33, ADJUST_POS_IDLE_WEAPONOUT);
			m_attachComp->SetAdjustAngles(Vector3(-80.0f, -20.0f, 80.0f));
			m_socketNum = 33;

			SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
			m_recomputeWorldTransform = false;
			m_worldTransform = Matrix4::CreateScale(m_scale)
				* Matrix4::CreateTranslation(ADJUST_POS_IDLE_WEAPONOUT)
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(80.0f)))
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(-20.0f)))
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(-80.0f)))
				* _boneWorldMat;

			return;
		}

	}

	// アタッチするボーンの指定と微調整用座標の更新
	m_attachComp->ChangeAttachBoneNum(1, ADJUST_POS_BASIC);
	m_attachComp->SetAdjustAngles(Vector3(-90.0f, 20.0f, 90.0f));
	m_socketNum = 1;

	SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
	m_recomputeWorldTransform = false;
	m_worldTransform = Matrix4::CreateScale(m_scale)
		* Matrix4::CreateTranslation(ADJUST_POS_BASIC)
		* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(-90.0f)))
		* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(90.0f)))
		* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(20.0f)))
		* _boneWorldMat;
}



