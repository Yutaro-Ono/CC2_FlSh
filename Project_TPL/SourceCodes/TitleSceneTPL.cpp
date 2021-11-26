#include "TitleSceneTPL.h"
#include "GameMain.h"
#include "WorldTitleSceneTPL.h"
#include "CanvasTitleSceneTPL.h"
#include "RenderBloom.h"
#include "LoadScreen.h"
#include "GameSceneTPL.h"
#include "AudioManager.h"
#include "ParticleManager.h"

TitleSceneTPL::TitleSceneTPL()
	:m_sceneState(SCENE_STATE::INTRO_FADEIN)
	,m_selectState(SELECT_STATE::NONE)
	,m_canvas(nullptr)
	,m_world(nullptr)
	,m_fadeInSpeed(0.15f)
{
}

TitleSceneTPL::~TitleSceneTPL()
{
	delete m_world;
	delete m_canvas;
	// �S�ẴA�N�^�[���폜
	GAME_INSTANCE.DeadAllActor();
	// �S�Ă�UI��Close�ɐݒ�
	for (auto iter : GAME_INSTANCE.GetUIStack())
	{
		iter->Close();
	}
	// �p�[�e�B�N����S�č폜
	RENDERER->GetParticleManager()->AllDeadParticle();
	// ���y���~
	AUDIO->StopMusic();
}

void TitleSceneTPL::Initialize()
{
	// ���[�h��ʂ̗L����
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}


	// �^�C�g���V�[���p���[���h�̐���
	m_world = new WorldTitleSceneTPL(this);
	m_world->Load();

	// UI�L�����o�X�̐���
	m_canvas = new CanvasTitleSceneTPL(m_world, this);
	m_canvas->Load();


	// ���[�h���o(�c�蕪)
	for (int i = 0; i < GAME_INSTANCE.GetLoadScreen()->GetGaugeAllNum(); i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}


	// ���[�h��ʂ̖�����
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
}


SceneBase* TitleSceneTPL::Update(float _deltaTime)
{
	m_world->Update(_deltaTime);
	m_world->UpdateWorld(_deltaTime);

	m_canvas->Update(_deltaTime);

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
		if (RENDERER->GetBloom()->FadeIn(m_fadeInSpeed, _deltaTime))
		{
			// �I��������I����ʂ��J�n
			m_sceneState = SCENE_STATE::SELECT;
			m_selectState = SELECT_STATE::PRESS_ANY_KEY;
			// UI�̃^�C�g�����o
			m_canvas->EnterTitle();
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
			GameSceneTPL* gameScene = new GameSceneTPL();

			return gameScene;
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

	// ����L�[�̎擾(�����ė�������)
	bool enter = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_E) ||
		CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B) ||
		CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_Y) ||
		CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START));

	// �L�����Z���L�[
	bool back = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B) ||
		CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X));

	// �����I������Ă��Ȃ�(Press Any Key)
	if (m_selectState == SELECT_STATE::PRESS_ANY_KEY)
	{
		if (enter)
		{
			m_selectState = SELECT_STATE::GAME_START;

			m_canvas->EnterSelect();

			return;
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
			return;
		}

		// ���L�[���͂����m������A����̑I�����ڂ�
		if (down)
		{
			m_selectState = SELECT_STATE::QUIT;
			return;
		}

		// ����L�[�����ꂽ��t�F�[�h�A�E�g�������Q�[���V�[��
		if (enter)
		{
			m_sceneState = SCENE_STATE::OUTRO_FADEOUT;
			return;
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

		// ����L�[�����ꂽ��Q�[���I��
		if (enter)
		{
			GAME_INSTANCE.SetShutDown();
			return;
		}
	}

	// �L�����Z���L�[�ōŏ��ɖ߂�
	if (back)
	{
		m_sceneState = SCENE_STATE::INTRO_FADEIN;
		m_selectState = SELECT_STATE::PRESS_ANY_KEY;

		m_canvas->BackTitle();

		m_fadeInSpeed = 0.4f;

		return;
	}
}

void TitleSceneTPL::Draw()
{
	RENDERER->Draw();
}
