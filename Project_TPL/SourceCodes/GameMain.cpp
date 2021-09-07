#include "GameMain.h"
#include "SceneBase.h"
#include "AudioManager.h"
#include "PhysicsWorld.h"
#include "Collision.h"
#include "Debugger.h"
#include "Camera.h"
#include "Actor.h"
#include "ActorPool.h"
#include "MeshPool.h"
#include "TexturePool.h"
#include "UIScreen.h"
#include "Camera.h"
#include "CameraComponent.h"
#include "DebugBox.h"
#include "ParticleManager.h"
#include "Font.h"
#include "PauseScreen.h"
#include "LoadScreen.h"
#include <algorithm>
#include <utility>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <sstream>
#include <document.h>

// �W���t�H���g
const std::string GameMain::FONT_FILE_PATH = "Data/Fonts/gang_wolfik.ttf";

// �R���X�g���N�^
GameMain::GameMain()
	:m_state(PLAYING)
	,m_renderer(nullptr)
	,m_audio(nullptr)
	,m_config(nullptr)
	,m_isRunning(true)
	,m_isPauseMode(false)
	,m_activeCamera(nullptr)
	,m_nowScene(nullptr)
	,m_physicsWorld(nullptr)
	,m_pauseScreen(nullptr)
	,m_loadScreen(nullptr)
	,m_debugger(nullptr)
	,m_actorPool(nullptr)
	,m_meshPool(nullptr)
	,m_texturePool(nullptr)
{
	// �����Ȃ�
}

// �f�X�g���N�^
GameMain::~GameMain()
{
	// �����Ȃ�
}

// �e�평��������(SDL, Renderer)
// in_full -> �t���X�N���[�����ǂ���
bool GameMain::Initialize()
{
	//--------------------------------------------------------------------+
	// SDL������
	//--------------------------------------------------------------------+
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("SDL Initialize Failed : %s\n", SDL_GetError());
		return false;
	}

	//--------------------------------------------------------------------+
    // �R���t�B�O
    //--------------------------------------------------------------------+
    // �R���t�B�O�̐����E���[�h
	m_config = new GameConfig();
	m_config->LoadConfig("Project_TPL.ini");

	//--------------------------------------------------------------------+
	// �e��I�u�W�F�N�g�v�[���N���X�̐���
	//--------------------------------------------------------------------+
	m_actorPool = new ActorPool();
	m_meshPool = new MeshPool();
	m_texturePool = new TexturePool();

	//--------------------------------------------------------------------+
	// �����_���[
	//--------------------------------------------------------------------+
	// �����_���[�̐���
	m_renderer = new Renderer();
	// �����_���[�̏�����
	if (!m_renderer->Initialize(GetConfig()->GetScreenWidth(), GetConfig()->GetScreenHeight(), GetConfig()->GetFullScreen()))
	{
		SDL_Log("Renderer Initialize Failed : %s\n", SDL_GetError());
		delete m_renderer;
		return false;
	}

    //--------------------------------------------------------------------+
    // �f�o�b�K�[ (Renderer��SDLWindow���쐬������)
    //--------------------------------------------------------------------+
#ifdef _DEBUG

	m_debugger = new Debugger(Debugger::BOTH_WINDOW);
	m_debugger->Initialize();

#endif

	// �����_���[�̃��[�h����(Debugger�𐶐�������)
	if (!m_renderer->Load())
	{
		SDL_Log("Renderer Load Failed : %s\n", SDL_GetError());
		delete m_renderer;
		return false;
	}

	//--------------------------------------------------------------------+
	// �t�H���g(TTF)�����_�����O�V�X�e��������
	//--------------------------------------------------------------------+
	if (TTF_Init() != 0)
	{
		SDL_Log("SDL_ttf Initialize Failed : %s\n", SDL_GetError());
		return false;
	}

	//--------------------------------------------------------------------+
	// �T�E���h�V�X�e��������
	//--------------------------------------------------------------------+
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		delete m_renderer;
		return false;
	}
	printf("SDLMixer����������\n");
	// SDLMixer API�������@44100:�����̎��g��, 44.1kHz 2:�X�e���I 4096:�����o�b�t�@�T�C�Y
	// ���ʉ���MP3�g�p�s��
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		return false;
	}
	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}
	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI����������\n");

	m_audio = new AudioManager();

	// �����蔻��V�X�e������
	m_physicsWorld = new PhysicsWorld;

	// �f�o�b�O�p�{�b�N�X
	m_debugBox = new DebugBox;

	// ���̓V�X�e��(�R���g���[���[)������
	if (!CONTROLLER_INSTANCE.Initialize())
	{
		return false;
	}

	// �t���[��������
	m_frame = 0;

	// SDL������������Ă���o�߂�������(�~���b�P��)
	m_ticksCount = SDL_GetTicks();

	// �t�H���g����
	Font* font = new Font();
	font->Load(FONT_FILE_PATH);
	m_fonts.emplace(FONT_FILE_PATH, font);

	// �|�[�Y��ʐ���
	m_pauseScreen = new PauseScreen();

	// ���[�h��ʐ���
	m_loadScreen = new LoadScreen();
	m_loadScreen->Initialize();



	return true;
}

// �e��������
void GameMain::Delete()
{

	// �e��v�[���̍폜
	m_actorPool->Delete();
	delete m_actorPool;
	m_meshPool->Delete();
	delete m_meshPool;
	m_texturePool->Delete();
	delete m_texturePool;

#ifdef _DEBUG

	// �f�o�b�K�[�̍폜
	m_debugger->Delete();
	delete m_debugger;

#endif

	// �����_���[�̍폜
	if (m_renderer)
	{
		m_renderer->Delete();
	}

	// UI�폜
	while (!m_uiStack.empty())
	{
		delete m_uiStack.back();
		m_uiStack.pop_back();
	}

	// �I�[�f�B�I�̍폜
	m_audio->ShutDown();
	delete m_audio;
	Mix_CloseAudio();
	Mix_Quit();

	delete m_physicsWorld;
	delete m_debugBox;

	// ���͊֘A�̍폜
	CONTROLLER_INSTANCE.Delete();

	// Imgui�̍폜
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_Quit();

}

// ���C�����[�v
void GameMain::RunLoop()
{
	// �J�n�V�[������`����Ă��Ȃ�������I��
	if (!m_nowScene)
	{
		std::cout << "ERROR::Scene is not set" << std::endl;
		return;
	}

	// �Q�[�����[�v
	// m_isRunning��false�Ȃ烋�[�v�𔲂��I��
	while (m_isRunning)
	{
		Input();
		// LoopHelper();

		if (UpdateGame())
		{
			continue;
		}

		// �`�揈��
		Draw();

#ifdef _DEBUG

		// �f�o�b�O�p�̕`�揈��(���f�o�b�O�r���h�̂�)
		//DebugRenderer();

#endif

	}
}

// �Q�[���̍X�V����
int GameMain::UpdateGame()
{
	// 16�~���b(= 60�t���[��/�b)�ɂȂ�悤�ɁA�O�̃t���[������16�~���b�ȏ�o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16));

	// �t���[���X�V
	m_frame++;

	// �O�̃t���[�����猻�ݎ����܂ł̌o�ߎ��ԎZ�o(�b�P�ʁj
	m_deltaTime = (SDL_GetTicks() - m_ticksCount + 16) / 1000.0f;

	//���̃t���[���̊J�n���ԕۑ��i���̃t���[���܂ł̌o�ߎ��Ԃ��͂��邽�߁j
	m_ticksCount = SDL_GetTicks();

	// �t���[�����Ԃ����܂�ɂ��o�߂��Ă���ꍇ��0.05 �� 20fps�Œ�
	if (m_deltaTime > 0.05f)
	{
		m_deltaTime = 0.05f;
	}

	if (!m_nowScene)
	{
		return 0;
	}

	// ���t���[���A�V�[������ۑ�
	SceneBase* tmpScene;

	// �A�N�^�[�̏���
	UpdateActor();

	// �J�����X�V
	if (m_activeCamera != nullptr)
	{
		// �J�����s��v�Z
		m_activeCamera->Update(m_deltaTime);
	}

	// �����蔻��
	m_physicsWorld->Collision();

	// �p�[�e�B�N���X�V
	m_renderer->GetParticleManager()->Update(m_deltaTime);

	//�V�[�������s
	tmpScene = m_nowScene->Update(m_deltaTime);

	// UI��CLOSE�Ȃ����
	auto iter = m_uiStack.begin();
	while (iter != m_uiStack.end())
	{
		if ((*iter)->GetState() == UIScreen::CLOSE)
		{
			delete *iter;
			iter = m_uiStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	// UI�̍X�V(CLOSE�Ȃ����)
	for (auto ui : m_uiStack)
	{
		if (ui->GetState() == UIScreen::ACTIVE)
		{
			ui->Update(m_deltaTime);
		}
	}

	// �V�[���X�V�̔��菈��
	if (tmpScene != m_nowScene)
	{

		// ���݂̃V�[�����폜
		delete m_nowScene;

		//�V�K�V�[�������̃V�[���ɍ����ւ�
		m_nowScene = tmpScene;

		// ����������V�K�V�[���p�ɍX�V����
		delete m_physicsWorld;
		m_physicsWorld = new PhysicsWorld();

		// ���[�h��ʐ���
		m_loadScreen = new LoadScreen();
		m_loadScreen->Initialize();

		// �V�[���̏�����
		m_nowScene->Initialize();

		// �|�[�Y��ʐ���
		m_pauseScreen = new PauseScreen();

		return 1;
	}


	return 0;

}

void GameMain::UpdateActor()
{
	// �S�A�N�^�[�X�V
	m_actorPool->UpdateObjects(m_deltaTime);
}


// �I������
void GameMain::CloseGame()
{
	Delete();
}

// �ŏ��̃V�[�����Z�b�g��������
void GameMain::SetFirstScene(SceneBase * in_scene)
{
	m_nowScene = in_scene;
	m_nowScene->Initialize();
}


void GameMain::Input()
{
	// �I���C�x���g�̃L���b�`
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_isRunning = false;
			break;

		default:
			CONTROLLER_INSTANCE.ReceiveEvent(event);
			break;
		}
	}

	// �L�[�{�[�h���͍X�V
	INPUT_INSTANCE.Update();

	// �R���g���[�����͍X�V
	CONTROLLER_INSTANCE.Update();

	// �}�E�X���͍X�V
	MOUSE_INSTANCE.Update();

	// ESC�������ꂽ��I��
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_ESCAPE))
	{
		m_isRunning = false;
	}

	// �|�[�Y���[�h�ڍs�^����
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_BACKSPACE) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START))
	{
		//�|�[�Y���[�h�ؑ�
		m_isPauseMode = !m_isPauseMode;
		Actor::State changeState;
		if (m_isPauseMode)
		{
			changeState = Actor::STATE_PAUSED;        // �|�[�Y
			m_pauseScreen->SetModeON();                     // �|�[�Y��ʂ��I��
		}
		else
		{
			m_pauseScreen->SetModeOFF();                    // �|�[�Y��ʂ��I�t
			changeState = Actor::STATE_ACTIVE;        // �A�N�e�B�u
		}

		//�S�ẴA�N�^�[�̏�Ԃ�ύX
		m_actorPool->SetActorState(changeState);
	}
}

void GameMain::SetDrawDebugBox(OBB in_box)
{
	if (!m_debugBox)
	{
		return;
	}

	m_debugBox->SetDebugBox(in_box);
}

void GameMain::DrawDebugBoxes()
{
	if (!m_debugBox)
	{
		return;
	}

	m_debugBox->Draw();
}

SDL_Renderer* GameMain::GetSDLRenderer()
{
	return m_renderer->GetSDLRenderer();
}

// �A�N�^�[�̒ǉ�����
void GameMain::AddActor(Actor * _actor)
{
	// ���[���h���W���X�V
	_actor->ComputeWorldTransform();
	// �A�N�^�[�v�[���֒ǉ�
	m_actorPool->AddObject(_actor);
}

// �A�N�^�[�̍폜����
void GameMain::RemoveActor(Actor * _actor)
{
	// �w�肳�ꂽ�A�N�^�[���v�[������폜
	m_actorPool->DeleteObject(_actor);
}

void GameMain::DeadAllActor()
{
	m_actorPool->Delete();
}

// UI��ʂ̒ǉ�
void GameMain::AddUI(UIScreen * _screen)
{
	m_uiStack.emplace_back(_screen);
}

// UI�̔��](�|�[�Y��ʂ��őO�ʂɂ��邽�߂̏���)
void GameMain::SwapPauseUI()
{
	m_pauseScreen->Close();

	m_pauseScreen = new PauseScreen();
}

// �����̃L�[�ƂȂ�t�@�C���p�X�Ńt�H���g�z����������A��v�����t�H���g��Ԃ�
// ������Ȃ������ꍇ�A�V�K�Ƀt�H���g�𐶐����A�z��ɒǉ���A�t�H���g��Ԃ�
Font* GameMain::GetFont(const std::string & _keyPath)
{
	// �����t�H���g�����łɔz��ɂȂ�������
	auto iter = m_fonts.find(_keyPath);

	// �����t�H���g�������Ă�����
	if (iter != m_fonts.end())
	{
		// �����t�H���g��Ԃ�
		return iter->second;
	}
	// �t�H���g�������Ă��Ȃ�������t�H���g�𐶐����A���������t�H���g��Ԃ�
	else
	{
		Font* font = new Font();

		if (font->Load(_keyPath))
		{
			m_fonts.emplace(_keyPath, font);
		}
		else
		{
			font->UnLoad();
			delete font;

			font = nullptr;
		}
		return font;
	}


}

// �e�L�X�g�̃��[�h
void GameMain::LoadText(const std::string & _fileName)
{
	// �e�L�X�g�}�b�v�̃N���A
	m_text.clear();
	// �w��p�X�̃t�@�C�����󂯎��
	std::ifstream file(_fileName);
	// �J���Ȃ�������
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found\n", _fileName.c_str());
		return;
	}

	// ��������擾
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	// rapidjson�ŊJ��
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON\n", _fileName.c_str());
		return;
	}
	// �e�L�X�g�}�b�v�ɕۊ�
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			m_text.emplace(itr->name.GetString(),
				itr->value.GetString());
		}
	}
}

// �e�L�X�g�̎擾
const std::string & GameMain::GetText(const std::string & _key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	//�}�b�v����w��̃L�[������
	auto iter = m_text.find(_key);
	if (iter != m_text.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}

// �J�����̃Z�b�g
void GameMain::SetCamera(Camera * _camera)
{

}

void GameMain::SetCamera(CameraComponent* _camera)
{
	m_activeCamera = _camera;
}

// �����̃J�������A�N�e�B�u�ɂ���(�����_���[�̃J������o�^)
void GameMain::InActiveCamera(Camera * _activeCam)
{
	//if (in_activeCam == m_activeCamera)
	//{
	//	printf("Camera is inActive, change to default view.\n");
	//	m_activeCamera = nullptr;
	//}

	//m_viewMatrix = 
	//	Matrix4::CreateLookAt(Vector3(0, 0, 100),
	//	Vector3(0, 0, 0),
	//	Vector3(0, 0, 1));
}

void GameMain::InActiveCamera(CameraComponent* _activeCam)
{
	if (_activeCam == m_activeCamera)
	{
		printf("Camera is inActive, change to default view.\n");
		m_activeCamera = nullptr;
	}

	m_viewMatrix =
		Matrix4::CreateLookAt(Vector3(0, 0, 100),
			Vector3(0, 0, 0),
			Vector3(0, 0, 1));
}

const Vector3 & GameMain::GetViewVector()
{
	if (m_activeCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
		return Vector3::Zero;
	}
	return m_activeCamera->GetViewMatrix().GetTranslation();

}




void GameMain::Draw()
{
	if (!m_nowScene)
	{
		return;
	}

	m_nowScene->Draw();
}

// �Q�[�����[�v����p�̃w���p�[�֐�
void GameMain::LoopHelper()
{
	SDL_Event event;
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
	switch (SDL_PollEvent(&event))
	{
		// �E�B���h�E�����(x���N���b�N����)�ꍇ�A���C�����[�v���I��
		case SDL_QUIT:
			m_isRunning = false;
			break;
	}

	// �L�[�{�[�h�̉�����Ԃ��擾
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	// ESC�L�[�Ń��C�����[�v�𔲂���
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		m_isRunning = false;
	}
}

/// <summary>
/// �f�o�b�O(imgui)�p�̕`�揈��
/// </summary>
void GameMain::DebugRenderer()
{
#ifdef _DEBUG

	// �f�o�b�K�[(imgui)�̍X�V
	m_debugger->UpdateImGui(m_deltaTime);

	// �f�o�b�K�[�̕`��
	m_debugger->RenderImGui();

#endif
}
