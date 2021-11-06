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
	// ���x���̍X�V
	m_label = u8"Weapon : AR4 : ID : " + std::to_string(m_weaponAR4->m_ID);

	// ��؂��
	ImGui::Separator();

	if (ImGui::TreeNode(m_label.c_str()))
	{
		// �e�ύX���ڃ��x��
		std::string itemLabel;

		//---------------------------------------------------------------------------+
		// �z���X�^�[���̒������W(�w��)
		//---------------------------------------------------------------------------+
		ImGui::Text(u8"����z���X�^�[���̍��W");
		Vector3 holSpinePos = m_weaponAR4->m_adjustHolsteredIdlePos;

		itemLabel = "Holster Spine X (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &holSpinePos.x, 0.5f);
		itemLabel = "Holster Spine Y (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &holSpinePos.y, 0.5f);
		itemLabel = "Holster Spine Z (" + m_label + ")";
		ImGui::InputFloat(itemLabel.c_str(), &holSpinePos.z, 0.5f);

		// �X�V���A�ύX�������W���Z�b�g
		if (holSpinePos.x != m_weaponAR4->m_adjustHolsteredIdlePos.x || holSpinePos.y != m_weaponAR4->m_adjustHolsteredIdlePos.y || holSpinePos.z != m_weaponAR4->m_adjustHolsteredIdlePos.z)
		{
			m_weaponAR4->m_adjustHolsteredIdlePos = holSpinePos;
		}

		//-----------------------------------+
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// ��z���X�^�[/�ҋ@���̒������W
		//---------------------------------------------------------------------------+
		ImGui::Text(u8"����o��/�ҋ@���̍��W");
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

		// �X�V���A�ύX�������W���Z�b�g
		if (UnholSpinePos.x != m_weaponAR4->m_adjustUnholsteredIdlePos.x || UnholSpinePos.y != m_weaponAR4->m_adjustUnholsteredIdlePos.y || UnholSpinePos.z != m_weaponAR4->m_adjustUnholsteredIdlePos.z)
		{
			m_weaponAR4->m_adjustUnholsteredIdlePos = UnholSpinePos;
		}
		//-----------------------------------+
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// ��z���X�^�[/�ړ����̒������W(�E��)
		//---------------------------------------------------------------------------+
		ImGui::Text(u8"����o��/�ړ����̍��W");
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

		// �X�V���A�ύX�������W���Z�b�g
		if (UnholHandPos.x != m_weaponAR4->m_adjustUnholsteredMovePos.x || UnholHandPos.y != m_weaponAR4->m_adjustUnholsteredMovePos.y || UnholHandPos.z != m_weaponAR4->m_adjustUnholsteredMovePos.z)
		{
			m_weaponAR4->m_adjustUnholsteredMovePos = UnholHandPos;
		}
		//-----------------------------------+
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		//�G�C�����̒������W(�E��)
		//---------------------------------------------------------------------------+
		ImGui::Text(u8"�G�C�����̍��W");
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

		// �X�V���A�ύX�������W���Z�b�g
		if (AimPos.x != m_weaponAR4->m_adjustAimPos.x || AimPos.y != m_weaponAR4->m_adjustAimPos.y || AimPos.z != m_weaponAR4->m_adjustAimPos.z)
		{
			m_weaponAR4->m_adjustAimPos = AimPos;
		}
		//-----------------------------------+
		//---------------------------------------------------------------------------+


		//---------------------------------------------------------------------------+
		// �z���X�^�[���̉�]�p(Y��)
		ImGui::Text(u8"�z���X�^�[���̉�]�p Y");
		float holsRadY = m_weaponAR4->m_holsterRadianY;

		itemLabel = "Holster Idle Radian Y (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &holsRadY, -360.0f, 360.0f);


		// �X�V���ꂽ��
		if (holsRadY != m_weaponAR4->m_holsterRadianY)
		{
			m_weaponAR4->m_holsterRadianY = holsRadY;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// ��z���X�^�[��/�ҋ@��Ԃ̉�]�p(X��)
		ImGui::Text(u8"����o��/�ҋ@���̉�]�p X");
		float unholsIdleRadX = m_weaponAR4->m_unholsterIdleRadianX;

		itemLabel = "Unholster Idle Radian X (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsIdleRadX, -360.0f, 360.0f);


		// �X�V���ꂽ��
		if (unholsIdleRadX != m_weaponAR4->m_unholsterIdleRadianX)
		{
			m_weaponAR4->m_unholsterIdleRadianX = unholsIdleRadX;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// ��z���X�^�[��/�ҋ@��Ԃ̉�]�p(Y��)
		ImGui::Text(u8"����o��/�ҋ@���̉�]�p Y");
		float unholsIdleRadY = m_weaponAR4->m_unholsterIdleRadianY;

		itemLabel = "Unholster Idle Radian Y (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsIdleRadY, -360.0f, 360.0f);


		// �X�V���ꂽ��
		if (unholsIdleRadY != m_weaponAR4->m_unholsterIdleRadianY)
		{
			m_weaponAR4->m_unholsterIdleRadianY = unholsIdleRadY;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// ��z���X�^�[��/�ҋ@��Ԃ̉�]�p(Z��)
		ImGui::Text(u8"����o��/�ҋ@���̉�]�p Z");
		float unholsIdleRadZ = m_weaponAR4->m_unholsterIdleRadianZ;

		itemLabel = "Unholster Idle Radian Z (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsIdleRadZ, -360.0f, 360.0f);


		// �X�V���ꂽ��
		if (unholsIdleRadZ != m_weaponAR4->m_unholsterIdleRadianZ)
		{
			m_weaponAR4->m_unholsterIdleRadianZ = unholsIdleRadZ;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// ��z���X�^�[��/�ړ���Ԃ̉�]�p(X��)
		ImGui::Text(u8"����o��/�ړ����̉�]�p X");
		float unholsMoveRadX = m_weaponAR4->m_unholsterMoveRadianX;

		itemLabel = "Unholster Move Radian X (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsMoveRadX, -360.0f, 360.0f);


		// �X�V���ꂽ��
		if (unholsMoveRadX != m_weaponAR4->m_unholsterMoveRadianX)
		{
			m_weaponAR4->m_unholsterMoveRadianX = unholsMoveRadX;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// ��z���X�^�[��/�ړ���Ԃ̉�]�p(Y��)
		ImGui::Text(u8"����o��/�ړ����̉�]�p Y");
		float unholsMoveRadY = m_weaponAR4->m_unholsterMoveRadianY;

		itemLabel = "Unholster Move Radian Y (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsMoveRadY, -360.0f, 360.0f);


		// �X�V���ꂽ��
		if (unholsMoveRadY != m_weaponAR4->m_unholsterMoveRadianY)
		{
			m_weaponAR4->m_unholsterMoveRadianY = unholsMoveRadY;
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// ��z���X�^�[��/�ړ���Ԃ̉�]�p(Z��)
		ImGui::Text(u8"����o��/�ړ����̉�]�p Z");
		float unholsMoveRadZ = m_weaponAR4->m_unholsterMoveRadianZ;

		itemLabel = "Unholster Move Radian Z (" + m_label + ")";
		ImGui::SliderFloat(itemLabel.c_str(), &unholsMoveRadZ, -360.0f, 360.0f);


		// �X�V���ꂽ��
		if (unholsMoveRadZ != m_weaponAR4->m_unholsterMoveRadianZ)
		{
			m_weaponAR4->m_unholsterMoveRadianZ = unholsMoveRadZ;
		}
		//---------------------------------------------------------------------------+

		// �f�o�b�O�E�B���h�E�ւ̃��b�V���̕`��
		Render();

		ImGui::TreePop();
	}
}

void DebugWeaponAR4::Render()
{
}
