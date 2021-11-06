#include "WeaponAR4.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "AttachWeaponToBoneComponent.h"
#include "Player.h"
#include "FirstPersonCameraComponent.h"
#include "Debugger.h"
#include "DebugWeaponAR4.h"

// オーナーアクターのボーンに武器をスナップする場合のボーン番号
const unsigned int WeaponAR4::SOCKET_NUM_SPINE = 1;          // 背骨
const unsigned int WeaponAR4::SOCKET_NUM_RIGHTHAND = 33;     // 右手
const unsigned int WeaponAR4::SOCKET_NUM_LEFTHAND = 10;      // 左手

// ボーンスナップ時の座標調整用ベクトル
const Vector3 WeaponAR4::ADJUST_POS_BASIC = Vector3(-101.0f, -14.0f, 46.5f);
const Vector3 WeaponAR4::ADJUST_POS_IDLE_WEAPONOUT = Vector3(26.5f, 107.0f, 60.0f);
const Vector3 WeaponAR4::ADJUST_POS_MOVE_WEAPONOUT = Vector3(-65.0f, 35.5f, -100.0f);

//const std::string WeaponAR4::AR4_MESH_PATH = "Data/Meshes/Weapons/AR4/SM_AR4.gpmesh";
const std::string WeaponAR4::AR4_MESH_PATH = "Data/Meshes/Weapons/SG_SP12/SG_SP12.gpmesh";


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
	// 調整座標のセット
	m_adjustHolsteredIdlePos = Vector3(-101.0f, -14.0f, 46.5f);
	m_holsterIdleRot = Quaternion::Identity;
	m_holsterRadianY = 67.5f;
	m_adjustUnholsteredIdlePos = Vector3(26.5f, 107.0f, 60.0f);
	m_unholsterIdleRadianX = 68.7f;
	m_unholsterIdleRadianY = 12.6f;
	m_unholsterIdleRadianZ = 191.8f;
	m_adjustUnholsteredMovePos = Vector3(-65.0f, 35.5f, -100.0f);
	m_unholsterMoveRadianX = 5.7f;
	m_unholsterMoveRadianY = -195.0f;
	m_unholsterMoveRadianZ = -86.0f;
	m_adjustAimPos = Vector3(0.0f, 0.0f, 0.0f);

	// AR4のメッシュをロード
	Mesh* mesh = RENDERER->GetMesh(AR4_MESH_PATH);
	
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// アタッチコンポーネント
	m_attachComp = new AttachWeaponToBoneComponent(this, m_ownerPlayer->GetSkelComp(), SOCKET_NUM_SPINE);

	//m_position = Vector3(0.0f, 10.0f, 103.0f);
	m_position = Vector3(3.0f, 0.0f, 102.5f);
	//m_scale = Vector3(0.85f, 0.85f, 0.85f);

	m_scale = Vector3(0.65f, 0.65f, 0.65f);


	// デバッグオブジェクトの生成(AR4固有)
#ifdef _DEBUG

	DebugWeaponAR4* debug = new DebugWeaponAR4(this);
	DEBUGGER->AddDebugObject(debug, m_tag);

#endif
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
			m_socketNum = 10;
			m_attachComp->ChangeAttachBoneNum(m_socketNum);

			SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));

			if (m_fpsCamera != nullptr)
			{
				Vector3 forwardVec = m_fpsCamera->GetCameraForwardVec();
				//forwardVec.z = 0.0f;
				RotateToNewForward(forwardVec);
			}


			// カメラの座標・ヨー・ピッチ情報を取得
			Vector3 cpos = m_fpsCamera->GetPosition();
			float yaw = m_fpsCamera->GetYaw();
			float pitch = m_fpsCamera->GetPitch();
			Vector3 rotatePos;
			rotatePos.x = cosf(pitch) * cosf(yaw);
			rotatePos.y = cosf(pitch) * sinf(yaw);
			rotatePos.z = sinf(pitch);

			// ワールド行列に合成
			m_worldTransform =
				Matrix4::CreateTranslation(m_adjustAimPos)
				* Matrix4::CreateRotationY(rotatePos.z)
				* Matrix4::CreateFromQuaternion(m_ownerPlayer->GetRotation())
				* Matrix4::CreateTranslation(cpos);

			m_recomputeWorldTransform = false;

			return;
		}


		// 待機状態と移動状態ではスナップするソケットを変更
		// 移動状態
		if (m_ownerPlayer->GetPlayerState() >= PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD &&
			m_ownerPlayer->GetPlayerState() <= PLAYER_STATE::STATE_WEAPONOUT_SPRINT)
		{
			// アタッチするボーンの指定と微調整用座標の更新
			m_socketNum = 10;
			m_attachComp->ChangeAttachBoneNum(m_socketNum);

			SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
			m_recomputeWorldTransform = false;
			m_worldTransform = Matrix4::CreateScale(m_scale)
				* Matrix4::CreateTranslation(m_adjustUnholsteredMovePos)
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(m_unholsterMoveRadianX)))
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(m_unholsterMoveRadianY)))
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(m_unholsterMoveRadianZ)))
				* _boneWorldMat;

			return;
		}
		// 待機状態
		else if (m_ownerPlayer->GetPlayerState() == PLAYER_STATE::STATE_WEAPONOUT_IDLE)
		{
			// アタッチするボーンの指定と微調整用座標の更新
			m_socketNum = 33;
			m_attachComp->ChangeAttachBoneNum(m_socketNum);

			SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
			m_recomputeWorldTransform = false;
			m_worldTransform = Matrix4::CreateScale(m_scale)
				* Matrix4::CreateTranslation(m_adjustUnholsteredIdlePos)
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitX, Math::ToRadians(m_unholsterIdleRadianX)))
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(m_unholsterIdleRadianY)))
				* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::ToRadians(m_unholsterIdleRadianZ)))
				* _boneWorldMat;

			return;
		}

	}

	// アタッチするボーンの指定と微調整用座標の更新
	m_socketNum = 1;
	m_attachComp->ChangeAttachBoneNum(m_socketNum);

	SetSocketMat(m_owner->GetSkelComp()->GetBoneMat(m_socketNum));
	m_recomputeWorldTransform = false;
	m_worldTransform = Matrix4::CreateScale(m_scale)
		* Matrix4::CreateTranslation(m_adjustHolsteredIdlePos)
		* Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitY, Math::ToRadians(m_holsterRadianY)))
		* _boneWorldMat;
}



