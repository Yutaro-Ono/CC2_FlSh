#include "MotorBikeParent.h"
#include "MeshComponent.h"
#include "GameMain.h"
#include "PointCameraComponent.h"

const std::string MotorBikeParent::STR_MOTORBIKE_BODY = "Data/Meshes/Actors/Vehicles/MotorBike/Main/SM_ClassicMotorBike_Main.OBJ";
const std::string MotorBikeParent::STR_MOTORBIKE_HANDLE = "Data/Meshes/Actors/Vehicles/MotorBike/HandleBar/SM_ClassicMotorBike_HandleBars.OBJ";
const std::string MotorBikeParent::STR_MOTORBIKE_WHEEL = "Data/Meshes/Actors/Vehicles/MotorBike/Wheel/SM_ClassicMotorBike_Wheel_Internal.OBJ";

MotorBikeParent::MotorBikeParent()
	:Actor(OBJECT_TAG::VEHICLE)
{
	// 修正座標
	m_offsetPos[static_cast<int>(MOTORBIKE_PARTS::BODY)] = Vector3::Zero;
	m_offsetPos[static_cast<int>(MOTORBIKE_PARTS::HANDLE)] = Vector3(0.0f, -50.0f, 50.0f);
	m_offsetPos[static_cast<int>(MOTORBIKE_PARTS::FRONT_WHEEL)] = Vector3(0.0f, 0.0f, 0.0f);
	m_offsetPos[static_cast<int>(MOTORBIKE_PARTS::BACK_WHEEL)] = Vector3(0.0f, 0.0f, 0.0f);

	// 各パーツの生成
	for (int i = 0; i < static_cast<int>(MOTORBIKE_PARTS::ALL); i++)
	{
		m_parts[i] = new Actor(OBJECT_TAG::VEHICLE);
		// ワールド行列の調整
		m_parts[i]->SetPosition(m_position + m_offsetPos[i]);
		m_parts[i]->SetScale(m_scale);
	}

	// 各パーツメッシュの生成とパーツアクターとの紐付け
	MeshComponent* bodyMesh = new MeshComponent(m_parts[static_cast<int>(MOTORBIKE_PARTS::BODY)]);
	bodyMesh->SetMesh(RENDERER->GetMesh(STR_MOTORBIKE_BODY));
	MeshComponent* handleMesh = new MeshComponent(m_parts[static_cast<int>(MOTORBIKE_PARTS::HANDLE)]);
	handleMesh->SetMesh(RENDERER->GetMesh(STR_MOTORBIKE_HANDLE));
	MeshComponent* wheelMeshF = new MeshComponent(m_parts[static_cast<int>(MOTORBIKE_PARTS::FRONT_WHEEL)]);
	wheelMeshF->SetMesh(RENDERER->GetMesh(STR_MOTORBIKE_WHEEL));
	MeshComponent* wheelMeshB = new MeshComponent(m_parts[static_cast<int>(MOTORBIKE_PARTS::BACK_WHEEL)]);
	wheelMeshB->SetMesh(RENDERER->GetMesh(STR_MOTORBIKE_WHEEL));

	// カメラ
	PointCameraComponent* camera = new PointCameraComponent(this, Vector3(65.0f, 75.0f, 60.0f));
	camera->SetMoveVec(Vector3(0.0f, 0.8f, 0.0f));
	camera->SetTargetOffset(Vector3(30.0f, 0.0f, 20.0f));
}

MotorBikeParent::~MotorBikeParent()
{
}

void MotorBikeParent::UpdateActor(float _deltaTime)
{
	
}
