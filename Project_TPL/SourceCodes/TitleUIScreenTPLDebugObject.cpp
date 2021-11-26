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
	// ラベルの更新
	m_label = "TitleScene : Interface";

	// 区切り線
	ImGui::Separator();

	if (ImGui::TreeNode(m_label.c_str()))
	{
		std::string itemLabel;

		// スクリーン座標
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

			// インプット形式
			std::string label;
			label = "Position X (" + itemLabel + ")";
			ImGui::InputFloat(label.c_str(), &pos.x, 1.0f, 0.0f);
			label = "Position Y (" + itemLabel + ")";
			ImGui::InputFloat(label.c_str(), &pos.y, 1.0f, 0.0f);

			// 更新されたら
			if (pos.x != uiPos.second.x || pos.y != uiPos.second.y)
			{
				m_owner->SetUIPosition(uiPos.first, pos);
			}
		}

		// スケール
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

			// インプット形式
			std::string label;
			label = "Scale (" + itemLabel + ")";
			ImGui::InputFloat(label.c_str(), &scale, 0.05f, 0.0f);

			// 更新されたら
			if (scale != uiScale.second)
			{
				m_owner->SetUIScale(uiScale.first, scale);
			}
		}

		// デバッグウィンドウへのメッシュの描画
		Render();

		ImGui::TreePop();
	}
}

void TitleUIScreenTPLDebugObject::Render()
{
}
