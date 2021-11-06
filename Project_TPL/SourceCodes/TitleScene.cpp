//-----------------------------------------------------------------------+
// �^�C�g���V�[���N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "TitleScene.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "Renderer.h"
#include "WorldTitleScene.h"
#include "TitleScreen.h"
#include "GameScene.h"
#include "LoadScreen.h"
#include "RenderBloom.h"
#include "TutorialScene.h"
#include "ActorPool.h"

const int TitleScene::STAGE_ALL_NUM = 1;

// �R���X�g���N�^
TitleScene::TitleScene()
	:m_state(FADE_IN)
	,m_selectedStage(0)
	,m_world(nullptr)
{
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
	GAME_INSTANCE.DeadAllActor();
}

// ����������
void TitleScene::Initialize()
{
	// ���[�h��ʂ̗L����
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}


	// ���[���h���̃��[�h
	m_world = new WorldTitleScene();
	m_world->Load();

	//--------------------------------------------------------------------------------------------+
	// ���y�֘A
	// SE
	m_sound["Enter"] = "Data/Music/SE/System/Enter/decide13.wav";
	AUDIO->GetSound(m_sound["Enter"]);                                       // ���艹
	m_sound["Select"] = "Data/Music/SE/System/Select/decide14.wav";
	AUDIO->GetSound(m_sound["Select"]);                                      // �I����
	// BGM
	m_sound["BGM"] = "Data/Music/BGM/Title/01_njol.wav";
	AUDIO->GetMusic(m_sound["BGM"]);

	// ���[�h���o(�c�蕪)
	for (int i = 0; i < GAME_INSTANCE.GetLoadScreen()->GetGaugeAllNum(); i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// ���[�h��ʂ̖�����
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();

	// �^�C�g���pUI
	//UIScreen* hud = new TitleScreen(this);

	// BGM�Đ��J�n
	AUDIO->PlayMusic(m_sound["BGM"]);
}

// �X�V����
SceneBase * TitleScene::Update(float _deltaTime)
{

	// ���[���h�̍X�V
	m_world->UpdateWorld(_deltaTime);

	return this;


	// �^�C�g�����j���[
	switch (m_state)
	{

	//----------------------------------------------------------------------+
    // "FADE IN"
    //----------------------------------------------------------------------+
	case FADE_IN:

		if (RENDERER->GetBloom()->FadeIn(0.15f, _deltaTime))
		{
			m_state = PRESS_ANY_KEY;
		}

		break;

	//----------------------------------------------------------------------+
    // "FADE OUT"
    //----------------------------------------------------------------------+
	case FADE_OUT:

		if (RENDERER->GetBloom()->FadeOut(0.7f, _deltaTime))
		{

            // �S�Ă�UI��Close�ɐݒ�
			for (auto iter : GAME_INSTANCE.GetUIStack())
			{
				iter->Close();
			}

			// �S�ẴA�N�^�[���폜
			for (auto actor : ACTOR_POOL->GetPool())
			{
				actor->SetState(Actor::STATE_DEAD);
			}

			// ���̃V�[����Ԃ�
			return new GameScene(m_selectedStage);
			//return new TutorialScene();
		}
		
		break;


	//----------------------------------------------------------------------+
	// "PRESS ANY KEY"
	//----------------------------------------------------------------------+
	case PRESS_ANY_KEY:


		// �����ꂩ�̃{�^���Ŏ���
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A)|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X)|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_Y)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Select"]);
			m_state = GAME_START;
		}

		break;

	//----------------------------------------------------------------------+
	// "GAME START"
	//----------------------------------------------------------------------+
	case GAME_START:

		// TAB��B�{�^�����������烁�j���[��ʖ߂�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = PRESS_ANY_KEY;
			break;
		}

		// ���L�[��DPAD���őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// �I����
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = GAME_QUIT;
		}

		//---------------------------------------------------------------------------+
        // �V�[���J�ڏ���
		//---------------------------------------------------------------------------+
        // SPACE��ENTER�AA�{�^�����������玟�̃V�[����
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);
			// ���y���t�F�[�h�A�E�g
			AUDIO->FadeOutMusic(3000.0f);

			// �t�F�[�h�A�E�g�����Ɉڍs
			m_state = FADE_OUT;
		}

		break;


	//-----------------------------------------------------------------------------------+
	// "STAGE SELECT"
	//-----------------------------------------------------------------------------------+
	case STAGE_SELECT:

		//---------------------------------------------------------------------+
		// �u�߂�v����
		//---------------------------------------------------------------------+
		// TAB��B�{�^�����������烁�j���[��ʖ߂�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Select"]);

			// "GAME_START"��
			m_state = GAME_START;

			break;
		}


		//-----------------------------------------------------------------------+
		// �X�e�[�W���ڂ̈ړ�������
		//-----------------------------------------------------------------------+
		// ���L�[��DPAD���őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// �I����
			AUDIO->PlaySoundTND(m_sound["Select"]);

			// �X�e�[�W1�łȂ���ΑI�𒆂̃X�e�[�W���J�E���g�_�E��
			if (m_selectedStage != 0)
			{
				m_selectedStage--;
			}

			break;
		}

		// ���L�[��DPAD���őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// �I����
			AUDIO->PlaySoundTND(m_sound["Select"]);

			// �X�e�[�W���̏���łȂ���ΑI�𒆂̃X�e�[�W�ԍ����J�E���g�A�b�v
			if (m_selectedStage < STAGE_ALL_NUM - 1)
			{
				m_selectedStage++;
			}

			break;
		}


		//---------------------------------------------------------------------------+
		// �V�[���J�ڏ���
		//---------------------------------------------------------------------------+
		// SPACE��ENTER�AA�{�^�����������玟�̃V�[����
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);
			// ���y���t�F�[�h�A�E�g
			AUDIO->FadeOutMusic(1.0f);

			// �v���C���[�̃T�E���h���~
			//m_player->AllStopSound();


			// �S�Ă�UI��Close�ɐݒ�
			for (auto iter : GAME_INSTANCE.GetUIStack())
			{
				iter->Close();
			}

			// �S�ẴA�N�^�[���폜
			for (auto actor : ACTOR_POOL->GetPool())
			{
				actor->SetState(Actor::STATE_DEAD);
			}

			// �t�F�[�h�A�E�g�����Ɉڍs
			m_state = FADE_OUT;
		}

		break;

	//----------------------------------------------------------------------+
	// "QUIT TO DESKTOP"
	//----------------------------------------------------------------------+
	case GAME_QUIT:


		// SPACE��ENTER�AA�{�^������������V���b�g�_�E��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			GAME_INSTANCE.SetShutDown();
			break;
		}

		// TAB��B�{�^�����������烁�j���[��ʖ߂�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = PRESS_ANY_KEY;
			break;
		}


		// ��L�[��DPAD��őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// �I����
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = GAME_START;
		}

		break;

	default:

		break;

	}

	
	return this;

}


// �`�揈��
void TitleScene::Draw()
{
	RENDERER->Draw();
}
