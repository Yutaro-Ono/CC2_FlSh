#include "DebugWeaponAR4.h"
#include "WeaponAR4.h"

DebugWeaponAR4::DebugWeaponAR4(WeaponAR4* _weaponAR4)
	:m_weaponAR4(_weaponAR4)
{
	m_label = u8"AR4::ID::" + std::to_string(m_weaponAR4->m_ID);
}

DebugWeaponAR4::~DebugWeaponAR4()
{
}

void DebugWeaponAR4::Update(float _deltaTime)
{
	// ラベルの更新
	m_label = u8"Weapon : AR4 : ID : " + std::to_string(m_weaponAR4->m_ID);

	// 区切り線
	ImGui::Separator();

	if (ImGui::TreeNode(m_label.c_str()))
	{
		// 各変更項目ラベル
		std::string itemLabel;

		//---------------------------------------------------------------------------+
		// ホルスター時の調整座標(背中)
		//---------------------------------------------------------------------------+
		ImGui::Text(u8"武器ホルスター時の座標");
		Vector3 holSpinePos = m_weaponAR4->m_adjustHolsteredIdlePos;

		itemLabel = "Holster Spine X (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &holSpinePos.x, 0.5f);
		itemLabel = "Holster Spine Y (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &holSpinePos.y, 0.5f);
		itemLabel = "Holster Spine Z (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &holSpinePos.z, 0.5f);

		// 更新時、変更した座標をセット
		if (holSpinePos.x != m_weaponAR4->m_adjustHolsteredIdlePos.x || holSpinePos.y != m_weaponAR4->m_adjustHolsteredIdlePos.y || holSpinePos.z != m_weaponAR4->m_adjustHolsteredIdlePos.z)
		{
			m_weaponAR4->m_adjustHolsteredIdlePos = holSpinePos;
		}

		//-----------------------------------+
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// 非ホルスター/待機時の調整座標
		//---------------------------------------------------------------------------+
		ImGui::Text(u8"武器出し/待機時の座標");
		Vector3 UnholSpinePos;
		UnholSpinePos.x = m_weaponAR4->m_adjustUnholsteredIdlePos.x;
		UnholSpinePos.y = m_weaponAR4->m_adjustUnholsteredIdlePos.y;
		UnholSpinePos.z = m_weaponAR4->m_adjustUnholsteredIdlePos.z;

		itemLabel = "Unholster Spine X (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &UnholSpinePos.x, 0.5f);
		itemLabel = "Unholster Spine Y (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &UnholSpinePos.y, 0.5f);
		itemLabel = "Unholster Spine Z (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &UnholSpinePos.z, 0.5f);

		// 更新時、変更した座標をセット
		if (UnholSpinePos.x != m_weaponAR4->m_adjustUnholsteredIdlePos.x || UnholSpinePos.y != m_weaponAR4->m_adjustUnholsteredIdlePos.y || UnholSpinePos.z != m_weaponAR4->m_adjustUnholsteredIdlePos.z)
		{
			m_weaponAR4->m_adjustUnholsteredIdlePos = UnholSpinePos;
		}
		//-----------------------------------+
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// 非ホルスター/移動時の調整座標(右手)
		//---------------------------------------------------------------------------+
		ImGui::Text(u8"武器出し/移動時の座標");
		Vector3 UnholHandPos;
		UnholHandPos.x = m_weaponAR4->m_adjustUnholsteredMovePos.x;
		UnholHandPos.y = m_weaponAR4->m_adjustUnholsteredMovePos.y;
		UnholHandPos.z = m_weaponAR4->m_adjustUnholsteredMovePos.z;

		itemLabel = "Unholstered Hand Pos X (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &UnholHandPos.x, 0.5f);
		itemLabel = "Unholstered Spine Pos Y (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &UnholHandPos.y, 0.5f);
		itemLabel = "Unholstered Spine Pos Z (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &UnholHandPos.z, 0.5f);

		// 更新時、変更した座標をセット
		if (UnholHandPos.x != m_weaponAR4->m_adjustUnholsteredMovePos.x || UnholHandPos.y != m_weaponAR4->m_adjustUnholsteredMovePos.y || UnholHandPos.z != m_weaponAR4->m_adjustUnholsteredMovePos.z)
		{
			m_weaponAR4->m_adjustUnholsteredMovePos = UnholHandPos;
		}
		//-----------------------------------+
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		//エイム時の調整座標(右手)
		//---------------------------------------------------------------------------+
		ImGui::Text(u8"エイム時の座標");
		Vector3 AimPos;
		AimPos.x = m_weaponAR4->m_adjustAimPos.x;
		AimPos.y = m_weaponAR4->m_adjustAimPos.y;
		AimPos.z = m_weaponAR4->m_adjustAimPos.z;

		itemLabel = "Aim Pos X (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &AimPos.x, 0.5f);
		itemLabel = "Aim Pos Y (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &AimPos.y, 0.5f);
		itemLabel = "Aim Pos Z (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &AimPos.z, 0.5f);

		// 更新時、変更した座標をセット
		if (AimPos.x != m_weaponAR4->m_adjustAimPos.x || AimPos.y != m_weaponAR4->m_adjustAimPos.y || AimPos.z != m_weaponAR4->m_adjustAimPos.z)
		{
			m_weaponAR4->m_adjustAimPos = AimPos;
		}
		//-----------------------------------+
		//---------------------------------------------------------------------------+


		//---------------------------------------------------------------------------+
		// ホルスター時の回転角(Y軸)
		ImGui::Text(u8"ホルスター時の回転角 Y");
		float holsRadY = m_weaponAR4->m_holsterRadianY;

		itemLabel = "Holster Idle Radian Y (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &holsRadY, -360.0f, 360.0f);


		// 更新されたら
		if (holsRadY != m_weaponAR4->m_holsterRadianY)
		{
			m_weaponAR4->m_holsterRadianY = holsRadY;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// 非ホルスター時/待機状態の回転角(X軸)
		ImGui::Text(u8"武器出し/待機時の回転角 X");
		float unholsIdleRadX = m_weaponAR4->m_unholsterIdleRadianX;

		itemLabel = "Unholster Idle Radian X (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsIdleRadX, -360.0f, 360.0f);


		// 更新されたら
		if (unholsIdleRadX != m_weaponAR4->m_unholsterIdleRadianX)
		{
			m_weaponAR4->m_unholsterIdleRadianX = unholsIdleRadX;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// 非ホルスター時/待機状態の回転角(Y軸)
		ImGui::Text(u8"武器出し/待機時の回転角 Y");
		float unholsIdleRadY = m_weaponAR4->m_unholsterIdleRadianY;

		itemLabel = "Unholster Idle Radian Y (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsIdleRadY, -360.0f, 360.0f);


		// 更新されたら
		if (unholsIdleRadY != m_weaponAR4->m_unholsterIdleRadianY)
		{
			m_weaponAR4->m_unholsterIdleRadianY = unholsIdleRadY;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// 非ホルスター時/待機状態の回転角(Z軸)
		ImGui::Text(u8"武器出し/待機時の回転角 Z");
		float unholsIdleRadZ = m_weaponAR4->m_unholsterIdleRadianZ;

		itemLabel = "Unholster Idle Radian Z (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsIdleRadZ, -360.0f, 360.0f);


		// 更新されたら
		if (unholsIdleRadZ != m_weaponAR4->m_unholsterIdleRadianZ)
		{
			m_weaponAR4->m_unholsterIdleRadianZ = unholsIdleRadZ;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// 非ホルスター時/移動状態の回転角(X軸)
		ImGui::Text(u8"武器出し/移動時の回転角 X");
		float unholsMoveRadX = m_weaponAR4->m_unholsterMoveRadianX;

		itemLabel = "Unholster Move Radian X (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsMoveRadX, -360.0f, 360.0f);


		// 更新されたら
		if (unholsMoveRadX != m_weaponAR4->m_unholsterMoveRadianX)
		{
			m_weaponAR4->m_unholsterMoveRadianX = unholsMoveRadX;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// 非ホルスター時/移動状態の回転角(Y軸)
		ImGui::Text(u8"武器出し/移動時の回転角 Y");
		float unholsMoveRadY = m_weaponAR4->m_unholsterMoveRadianY;

		itemLabel = "Unholster Move Radian Y (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsMoveRadY, -360.0f, 360.0f);


		// 更新されたら
		if (unholsMoveRadY != m_weaponAR4->m_unholsterMoveRadianY)
		{
			m_weaponAR4->m_unholsterMoveRadianY = unholsMoveRadY;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// 非ホルスター時/移動状態の回転角(Z軸)
		ImGui::Text(u8"武器出し/移動時の回転角 Z");
		float unholsMoveRadZ = m_weaponAR4->m_unholsterMoveRadianZ;

		itemLabel = "Unholster Move Radian Z (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsMoveRadZ, -360.0f, 360.0f);


		// 更新されたら
		if (unholsMoveRadZ != m_weaponAR4->m_unholsterMoveRadianZ)
		{
			m_weaponAR4->m_unholsterMoveRadianZ = unholsMoveRadZ;
		}
		//---------------------------------------------------------------------------+

		// デバッグウィンドウへのメッシュの描画
		Render();

		ImGui::TreePop();
	}
}

void DebugWeaponAR4::Render()
{
}
