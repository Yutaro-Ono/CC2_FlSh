#include "TitleSceneTPL.h"
#include "WorldTitleSceneTPL.h"
#include "GameMain.h"
#include "RenderBloom.h"

TitleSceneTPL::TitleSceneTPL()
	:m_sceneState(SCENE_STATE::INTRO_FADEIN)
	,m_selectState(SELECT_STATE::NONE)
	,m_canvas(nullptr)
	,m_world(nullptr)
{
}

TitleSceneTPL::~TitleSceneTPL()
{
}

void TitleSceneTPL::Initialize()
{
	// �^�C�g���V�[���p���[���h�̐���
	m_world = new WorldTitleSceneTPL(this);
	m_world->Load();

}


SceneBase* TitleSceneTPL::Update(float _deltaTime)
{
	return UpdateSceneState(_deltaTime);
}

/// <summary>
/// �V�[���X�e�[�g�̍X�V����
/// </summary>
/// <param name="_deltaTime"> �f���^�^�C�� </param>
/// <returns> ���̃V�[���܂��͂��̃V�[���N���X�̃|�C���^ </returns>
SceneBase* TitleSceneTPL::UpdateSceneState(float _deltaTime)
{
	// �t�F�[�h�C��
	if (m_sceneState == SCENE_STATE::INTRO_FADEIN)
	{
		// �t�F�[�h�C������
		if (RENDERER->GetBloom()->FadeIn(0.15f, _deltaTime))
		{
			// �I��������I����ʂֈڍs
			m_sceneState = SCENE_STATE::SELECT;
		}

		return this;
	}

	// �I�����
	if (m_sceneState == SCENE_STATE::SELECT)
	{
		UpdateSelectState();

		return this;
	}

	// �t�F�[�h�A�E�g
	if (m_sceneState == SCENE_STATE::OUTRO_FADEOUT)
	{
		// �t�F�[�h�A�E�g����
		if (RENDERER->GetBloom()->FadeOut(0.7f, _deltaTime))
		{
			// �I�������玟�̃V�[���ֈڍs


			//return gameScene;
		}

		return this;
	}

	return this;
}

/// <summary>
/// �I����ʎ��A�I�����ڃX�e�[�g�̍X�V�֐�
/// </summary>
void TitleSceneTPL::UpdateSelectState()
{
	// �����I������Ă��Ȃ�(Press Any Key)
	if (m_selectState == SELECT_STATE::NONE)
	{
		// ����L�[�̎擾(�����ė�������)
		bool enter = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN)|| INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_E) ||
			          CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B)||
			          CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_Y)||
			          CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START));

		if (enter)
		{
			m_selectState == SELECT_STATE::GAME_START;
		}
	}

	if (m_selectState == SELECT_STATE::GAME_START)
	{
		// �㉺�L�[���͂̎擾
		bool up = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP));
		bool down = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN));

		// ��L�[���͂����m������A���̑I�����ڂ�
		if (up)
		{
			m_selectState = SELECT_STATE::QUIT;
		}

		// ���L�[���͂����m������A����̑I�����ڂ�
		if (down)
		{
			m_selectState = SELECT_STATE::QUIT;
		}
	}

	if (m_selectState == SELECT_STATE::QUIT)
	{
		// �㉺�L�[���͂̎擾
		bool up = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP));
		bool down = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN));

		// ��L�[���͂����m������A���̑I�����ڂ�
		if (up)
		{
			m_selectState = SELECT_STATE::GAME_START;
		}

		// ���L�[���͂����m������A����̑I�����ڂ�
		if (down)
		{
			m_selectState = SELECT_STATE::GAME_START;
		}
	}
}

void TitleSceneTPL::Draw()
{
}
