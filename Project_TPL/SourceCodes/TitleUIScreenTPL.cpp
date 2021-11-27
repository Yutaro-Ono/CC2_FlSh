#include "TitleUIScreenTPL.h"
#include "GameMain.h"
#include "TitleSceneTPL.h"
#include "Debugger.h"
#include "TitleUIScreenTPLDebugObject.h"
#include "Texture.h"

TitleUIScreenTPL::TitleUIScreenTPL(class TitleSceneTPL* _titleScene)
    :m_titleScene(_titleScene)
    ,m_debugObj(nullptr)
{
    // インターフェース用テクスチャのロード
    m_uiTextures[TITLEUI_CATEGORY::LOGO] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_TitleLogo.png");
    m_uiPos[TITLEUI_CATEGORY::LOGO] = Vector2(0.0f, -200.0f);
    m_uiScale[TITLEUI_CATEGORY::LOGO] = 0.4f;
    m_uiVisible[TITLEUI_CATEGORY::LOGO] = false;

    m_uiTextures[TITLEUI_CATEGORY::PRESS_ANY_KEY] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_PressAnyKey.png");
    m_uiPos[TITLEUI_CATEGORY::PRESS_ANY_KEY] = Vector2(0.0f, -425.0f);
    m_uiScale[TITLEUI_CATEGORY::PRESS_ANY_KEY] = 0.45f;
    m_uiVisible[TITLEUI_CATEGORY::PRESS_ANY_KEY] = false;


    m_uiTextures[TITLEUI_CATEGORY::CURSOL] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_Cursol.png");
    m_uiPos[TITLEUI_CATEGORY::CURSOL] = Vector2(0.0f, 0.0f);
    m_uiScale[TITLEUI_CATEGORY::CURSOL] = 1.0f;
    m_uiVisible[TITLEUI_CATEGORY::CURSOL] = false;


    m_uiTextures[TITLEUI_CATEGORY::GAME_START] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_GameStart.png");
    m_uiPos[TITLEUI_CATEGORY::GAME_START] = Vector2(-378.0f, 346.0f);
    m_uiScale[TITLEUI_CATEGORY::GAME_START] = 0.75f;
    m_uiVisible[TITLEUI_CATEGORY::GAME_START] = false;


    m_uiTextures[TITLEUI_CATEGORY::GAME_QUIT] = RENDERER->GetTexture("Data/Interface/TPL/Title/TPL_Quit.png");
    m_uiPos[TITLEUI_CATEGORY::GAME_QUIT] = Vector2(-546.0f, -418.0f);
    m_uiScale[TITLEUI_CATEGORY::GAME_QUIT] = 0.75f;
    m_uiVisible[TITLEUI_CATEGORY::GAME_QUIT] = false;



    // デバッグオブジェクトの生成
#ifdef _DEBUG

    m_debugObj = new TitleUIScreenTPLDebugObject(this);
    DEBUGGER->AddDebugObject(m_debugObj, Debugger::DEBUG_TAG::INTERFACE);

#endif

}

TitleUIScreenTPL::~TitleUIScreenTPL()
{

}

void TitleUIScreenTPL::Update(float _deltaTime)
{

    if (m_titleScene->GetSelectState() == TitleSceneTPL::SELECT_STATE::GAME_START)
    {
        SetCursolPosition(TITLEUI_CATEGORY::GAME_START);
    }

    if (m_titleScene->GetSelectState() == TitleSceneTPL::SELECT_STATE::QUIT)
    {
        SetCursolPosition(TITLEUI_CATEGORY::GAME_QUIT);
    }

}

void TitleUIScreenTPL::EnterTitle()
{
    // PRESS ANY KEY表示・それ以外のUI消す
    m_uiVisible[TITLEUI_CATEGORY::LOGO] = true;
    m_uiVisible[TITLEUI_CATEGORY::PRESS_ANY_KEY] = true;
    m_uiVisible[TITLEUI_CATEGORY::CURSOL] = false;
    m_uiVisible[TITLEUI_CATEGORY::GAME_START] = false;
    m_uiVisible[TITLEUI_CATEGORY::GAME_QUIT] = false;
}

void TitleUIScreenTPL::EnterSelect()
{
    m_uiVisible[TITLEUI_CATEGORY::LOGO] = false;
    m_uiVisible[TITLEUI_CATEGORY::PRESS_ANY_KEY] = false;
    m_uiVisible[TITLEUI_CATEGORY::CURSOL] = true;
    m_uiVisible[TITLEUI_CATEGORY::GAME_START] = true;
    m_uiVisible[TITLEUI_CATEGORY::GAME_QUIT] = true;
}

void TitleUIScreenTPL::BackTitle()
{
    m_uiVisible[TITLEUI_CATEGORY::CURSOL] = false;
    m_uiVisible[TITLEUI_CATEGORY::GAME_START] = false;
    m_uiVisible[TITLEUI_CATEGORY::GAME_QUIT] = false;
}

/// <summary>
/// カーソル座標のセット
/// </summary>
/// <param name="_uiCategory"> カーソルを合わせるUI </param>
void TitleUIScreenTPL::SetCursolPosition(TITLEUI_CATEGORY _uiCategory)
{
    Vector2 adjust = Vector2::Zero;

    // "QUIT"だった場合は座標を調整
    if (_uiCategory == TITLEUI_CATEGORY::GAME_QUIT)
    {
        adjust.x = m_uiTextures[_uiCategory]->GetHalfWidth() * m_uiScale[_uiCategory] / 2;
    }

    m_uiPos[TITLEUI_CATEGORY::CURSOL] = m_uiPos[_uiCategory] - adjust;
}

void TitleUIScreenTPL::SetUIPosition(TITLEUI_CATEGORY _uiCategory, const Vector2& _pos)
{
    m_uiPos[_uiCategory] = _pos;
}

void TitleUIScreenTPL::SetUIScale(TITLEUI_CATEGORY _uiCategory, float _scale)
{
    m_uiScale[_uiCategory] = _scale;
}

void TitleUIScreenTPL::Draw(GLSLprogram* _shader)
{
    // 全てのUIの描画
    for (auto ui : m_uiTextures)
    {
        if (m_uiVisible[ui.first])
        {
            DrawTexture(_shader, ui.second, m_uiPos[ui.first], m_uiScale[ui.first]);
        }

    }
}
