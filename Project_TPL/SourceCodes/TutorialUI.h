//-----------------------------------------------------------------------+
// チュートリアル用UI(ゲームシーン).
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"


class TutorialUI : public UIScreen
{


public:


	TutorialUI();                                     // コンストラクタ
	~TutorialUI();                                    // デストラクタ


	void Update(float _deltaTime) override;         // 更新処理

	void Draw(GLSLprogram* _shader) override;               // 描画処理


private:


	class Texture* m_controllerTex;                   // 操作説明 ※コントローラ用

	class Texture* m_keyboardTex;                     // 操作説明 ※キーボード用

	class Texture* m_taskTex;                         // タスク(目標の表示)


};