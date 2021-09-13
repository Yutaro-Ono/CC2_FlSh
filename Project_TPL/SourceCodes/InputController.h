//-----------------------------------------------------------------------+
// コントローラークラス(シングルトン)
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "GameMain.h"

class InputController
{


public:

	// インスタンス生成
	static InputController& ControllerInstance()
	{
		static InputController ControllerInstance;
		return ControllerInstance;
	}

	~InputController();          // デストラクタ

	bool Initialize();           // 初期化処理
	void Update();               // 更新処理
	void Delete();               // 解放処理

	// コントローラーを抜き差しした時
	void ReceiveEvent(const SDL_Event& event);

	// ボタンのステート取得関数
	bool IsTriggered(const SDL_GameControllerButton in_button) const;
	bool IsPressed(const SDL_GameControllerButton in_button) const;
	bool IsReleased(const SDL_GameControllerButton in_button) const;

	bool IsAvailable() { return m_gameController != nullptr; }

	// 現在の軸の値を取得
	float GetAxisValue(const SDL_GameControllerAxis in_axis) const;
	Vector2& GetLAxisVec() { return m_axisL; }
	Vector2& GetRAxisVec() { return m_axisR; }

	bool GetIsInputAxisL() { return m_isInputAxisL; }
	bool GetIsInputAxisR() { return m_isInputAxisR; }

private:

	InputController();           // コンストラクタ

	// コントローラー
	SDL_GameController* m_gameController;

	// コントローラーのステート
	Uint8 m_buttonStates[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 m_prevButtonStates[SDL_CONTROLLER_BUTTON_MAX];
	float m_axisValues[SDL_CONTROLLER_AXIS_MAX];
	
	int m_whichController;

	Vector2 m_axisL;               // 左スティック入力取得用
	Vector2 m_axisR;               // 右スティック入力取得用

	bool m_isInputAxisL;           // 左スティック入力がされているか
	bool m_isInputAxisR;           // 右スティックが入力されているか

	static const float PAD_MAX_VALUE;
	static const float STICK_AXIS_THRESHOLD;     // スティック入力閾値(これ以上なら入力を有効とみなす)
};

#define CONTROLLER_INSTANCE InputController::ControllerInstance()