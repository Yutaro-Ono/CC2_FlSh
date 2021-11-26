#include "TitleUIScreenTPLDebugObject.h"
#include "TitleUIScreenTPL.h"
#include <iostream>


TitleUIScreenTPLDebugObject::TitleUIScreenTPLDebugObject(TitleUIScreenTPL* _owner)
	:m_owner(_owner)
{
}

TitleUIScreenTPLDebugObject::~TitleUIScreenTPLDebugObject()
{
}

void TitleUIScreenTPLDebugObject::Update(float _deltaTime)
{
	// ���x���̍X�V
	m_label = "TitleScene : Interface";

	// ��؂��
	ImGui::Separator();

	if (ImGui::TreeNode(m_label.c_str()))
	{
		std::string itemLabel;

		// �X�N���[�����W
		for (auto uiPos : m_owner->m_uiPos)
		{
			switch (uiPos.first)
			{
			case TitleUIScreenTPL::TITLEUI_CATEGORY::LOGO:

				itemLabel = "Pos : TitleLogo";

				break;

			case TitleUIScreenTPL::TITLEUI_CATEGORY::PRESS_ANY_KEY:

				itemLabel = "Pos : PressAnyKey";

				break;

			case TitleUIScreenTPL::TITLEUI_CATEGORY::CURSOL:

				itemLabel = "Pos : Cursol";

				break;


			case TitleUIScreenTPL::TITLEUI_CATEGORY::GAME_START:

				itemLabel = "Pos : GameStart";

				break;

			case TitleUIScreenTPL::TITLEUI_CATEGORY::GAME_QUIT:

				itemLabel = "Pos : GameQuit";

				break;


			default:

				itemLabel = "Pos : Error";

				break;
			}

			ImGui::Text(itemLabel.c_str());
			Vector2 pos;
			pos.x = uiPos.second.x;
			pos.y = uiPos.second.y;

			// �C���v�b�g�`��
			std::string label;
			label = "Position X (" + itemLabel + ")";
			ImGui::InputFloat(label.c_str(), &pos.x, 1.0f, 0.0f);
			label = "Position Y (" + itemLabel + ")";
			ImGui::InputFloat(label.c_str(), &pos.y, 1.0f, 0.0f);

			// �X�V���ꂽ��
			if (pos.x != uiPos.second.x || pos.y != uiPos.second.y)
			{
				m_owner->SetUIPosition(uiPos.first, pos);
			}
		}

		// �X�P�[��
		for (auto uiScale : m_owner->m_uiScale)
		{

			switch (uiScale.first)
			{
			case TitleUIScreenTPL::TITLEUI_CATEGORY::LOGO:

				itemLabel = "Scale : TitleLogo";

				break;

			case TitleUIScreenTPL::TITLEUI_CATEGORY::PRESS_ANY_KEY:

				itemLabel = "Scale : PressAnyKey";

				break;

			case TitleUIScreenTPL::TITLEUI_CATEGORY::CURSOL:

				itemLabel = "Scale : Cursol";

				break;


			case TitleUIScreenTPL::TITLEUI_CATEGORY::GAME_START:

				itemLabel = "Scale : GameStart";

				break;

			case TitleUIScreenTPL::TITLEUI_CATEGORY::GAME_QUIT:

				itemLabel = "Scale : GameQuit";

				break;


			default:

				itemLabel = "Scale : Error";

				break;
			}

			ImGui::Text(itemLabel.c_str());
			float scale;
			scale = uiScale.second;

			// �C���v�b�g�`��
			std::string label;
			label = "Scale (" + itemLabel + ")";
			ImGui::InputFloat(label.c_str(), &scale, 0.05f, 0.0f);

			// �X�V���ꂽ��
			if (scale != uiScale.second)
			{
				m_owner->SetUIScale(uiScale.first, scale);
			}
		}

		// �f�o�b�O�E�B���h�E�ւ̃��b�V���̕`��
		Render();

		ImGui::TreePop();
	}
}

void TitleUIScreenTPLDebugObject::Render()
{
}
