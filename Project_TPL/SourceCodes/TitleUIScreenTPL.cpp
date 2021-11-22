#include "TitleUIScreenTPL.h"
#include "GameMain.h"


TitleUIScreenTPL::TitleUIScreenTPL()
{
    // インターフェース用テクスチャのロード
    m_uiTextures[TITLEUI_CATEGORY::LOGO] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_TitleLogo.png");
    m_uiPos[TITLEUI_CATEGORY::LOGO] = Vector2(0.0f, 0.0f);
    m_uiScale[TITLEUI_CATEGORY::LOGO] = 1.0f;

    m_uiTextures[TITLEUI_CATEGORY::PRESS_ANY_KEY] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_PressAnyKey.png");
    m_uiPos[TITLEUI_CATEGORY::PRESS_ANY_KEY] = Vector2(0.0f, 0.0f);
    m_uiScale[TITLEUI_CATEGORY::PRESS_ANY_KEY] = 1.0f;

    m_uiTextures[TITLEUI_CATEGORY::CURSOL] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_Cursol.png");
    m_uiPos[TITLEUI_CATEGORY::CURSOL] = Vector2(0.0f, 0.0f);
    m_uiScale[TITLEUI_CATEGORY::CURSOL] = 1.0f;

    m_uiTextures[TITLEUI_CATEGORY::GAME_START] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_GameStart.png");
    m_uiPos[TITLEUI_CATEGORY::GAME_START] = Vector2(0.0f, 0.0f);
    m_uiScale[TITLEUI_CATEGORY::GAME_START] = 1.0f;

    m_uiTextures[TITLEUI_CATEGORY::GAME_QUIT] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_Quit.png");
    m_uiPos[TITLEUI_CATEGORY::GAME_QUIT] = Vector2(0.0f, 0.0f);
    m_uiScale[TITLEUI_CATEGORY::GAME_QUIT] = 1.0f;

}

TitleUIScreenTPL::~TitleUIScreenTPL()
{
    //// テクスチャの解放
//for (auto i : m_uiTextures)
//{
//    i.second->Delete();
//    delete i.second;
//}
//m_uiTextures.clear();
}

void TitleUIScreenTPL::Update(float _deltaTime)
{
}

void TitleUIScreenTPL::Draw(GLSLprogram* _shader)
{
    // 全てのUIの描画
    for (auto ui : m_uiTextures)
    {
        DrawTexture(_shader, ui.second, m_uiPos[ui.first], m_uiScale[ui.first]);
    }
}
