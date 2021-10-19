#include"mouse.h"
#include <iostream>

Mouse::Mouse()
{
	m_mousePos = Vector2(0, 0);
	m_currentButtons = MouseButtonState::MOUSE_BUTTON_OFF;
	m_prevButtons = MouseButtonState::MOUSE_BUTTON_OFF;
	m_isRelative = true;

	// マウスの相対モードON
	SetRelativeMouseMode(m_isRelative);
}

void Mouse::SetRelativeMouseMode(bool _value)
{
	SDL_bool set = _value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);
	m_isRelative = _value;
}

void Mouse::SetMousePos(const Vector3& _mousePos)
{
	
}

bool Mouse::GetButtonValue(int _button) const
{
	return _button & m_currentButtons;
}

Mouse::MouseButtonState Mouse::GetButtonState(MouseButtonState _button) const
{

	if (_button & m_currentButtons)
	{
		return (_button & m_prevButtons) ? MOUSE_BUTTON_PRESSED : MOUSE_BUTTON_PUSHDOWN;
	}

	return (_button & m_prevButtons) ? MOUSE_BUTTON_PULLUP : MOUSE_BUTTON_OFF;
}

void Mouse::OnMouseWheelEvent(SDL_Event& _event)
{
	switch (_event.type)
	{
	case SDL_MOUSEWHEEL:
		m_mouseWheel = Vector2(
			static_cast<float>(_event.wheel.x),
			static_cast<float>(_event.wheel.y)
		);
		break;

	default:
		break;
	}
}

void Mouse::OnMouseClickEvent(SDL_Event& _event)
{
	switch (_event.type)
	{

	case SDL_MOUSEBUTTONDOWN:

		if (_event.button.button == SDL_BUTTON_RIGHT)
		{
			m_currentButtons = MouseButtonState::MOUSE_BUTTON_PRESSED;
		}

		break;

	case SDL_MOUSEBUTTONUP:

		if (_event.button.button == SDL_BUTTON_RIGHT)
		{
			m_currentButtons = MouseButtonState::MOUSE_BUTTON_OFF;
		}

		break;

	default:
		break;
	}
}

// 入力処理の更新。レンダリングループの先頭で1回だけ呼ぶようにする
void Mouse::Update()
{
	m_prevButtons = m_currentButtons;

	int x = 0, y = 0;
	if (m_isRelative)
	{
		SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		SDL_GetMouseState(&x, &y);
	}
	m_mousePos.x = static_cast<float>(x);
	m_mousePos.y = static_cast<float>(y);

	// ALTキーを押している時はレラティブモードをOFF
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LALT))
	{
		m_isRelative = false;
		SetRelativeMouseMode(m_isRelative);
	}
	if(INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LALT))
	{
		m_isRelative = true;
		SetRelativeMouseMode(m_isRelative);
	}

	m_mouseWheel = Vector2::Zero;
}

void Mouse::RenderDebugImGui()
{
	// ImGuiフレームを開始
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GAME_INSTANCE.GetRenderer()->GetSDLWindow());
	ImGui::NewFrame();

	// ImGui更新
	ImGui::Begin("Debug Console : Mouse Input");
	ImGui::SliderFloat("Mouse Position : x", &m_mousePos.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Mouse Position : y", &m_mousePos.y, 0.0f, 1.0f);

	ImGui::End();
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	//glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
