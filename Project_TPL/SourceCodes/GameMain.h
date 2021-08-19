//-----------------------------------------------------------------------+
// �Q�[�����[�v.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
// �C���N���[�h�t�@�C��
#include <unordered_map>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_types.h>
#include "Math.h"
#include "Input.h"
#include "InputController.h"
#include "InputSteeringWheel.h"
#include "Mouse.h"
#include "GameConfig.h"
#include "Renderer.h"
#include "Mouse.h"
#include "../imgui/imconfig.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_sdl.h"


class SceneBase;
class TestScene;

class GameMain
{


public:

	// �Q�[���̏�ԊǗ�
	enum GameState
	{
		PLAYING,
		PAUSED,
		QUIT
	};

	// �V���O���g��
	static GameMain& GetInstance()
	{
		static GameMain game;
		return game;
	}

	~GameMain();                                                                    // �f�X�g���N�^

	bool   Initialize();                // �e�평��������
	void   Delete();                                                                // �e��������

	void   RunLoop();                                                               // �Q�[���̃��C�����[�v
	void   CloseGame();                                                             // �Q�[���̏I������

	void   SetFirstScene(class SceneBase* in_scene);                                // �J�n�V�[���̃Z�b�g

	void SetShutDown() { m_isRunning = false; }                                     // �V���b�g�_�E������



	void AddActor(class Actor* in_actor);                                           // �A�N�^�[�̒ǉ�
	void RemoveActor(class Actor* in_actor);                                        // �A�N�^�[�̍폜
	void DeadAllActor();                                                            // �A�N�^�[�S�폜
	const std::vector<class Actor*>& GetActorStack() const { return m_actors; }     // �A�N�^�[�z��̎擾


    //--------------------------------------------------------------------//
	// Getter/Setter
	//-------------------------------------------------------------------//
	class GameConfig* GetConfig() const { return m_config; }

	class Renderer* GetRenderer() const { return m_renderer; }                       // �����_���[�擾
	SDL_Renderer* GetSDLRenderer();                                                  // 2D�pSDL�����_���[�̎擾
	// �J����
	void SetCamera(class Camera* in_camera);                                         // �J�������V�X�e���ɓo�^
	void SetCamera(class CameraComponent* in_camera);                                // �J�������V�X�e���ɓo�^
	class CameraComponent* GetCamera() { return m_activeCamera; }                    // �A�N�e�B�u�ȃJ�����|�C���^�̎擾

	void InActiveCamera(class Camera* in_activeCam);                                 // �J�����̓o�^������
	void InActiveCamera(class CameraComponent* in_activeCam);                        // �J�����̓o�^������
	// �s��擾
	const Matrix4& GetViewMatrix() { return m_viewMatrix; };                         // �r���[�s��̃Q�b�^�[
	const Vector3& GetViewVector();                                                  // �r���[���W�̃Q�b�^�[
	// �t���[���v��
	const int GetFrame() { return m_frame; }                                         // �t���[���̃Q�b�^�[
	// �f���^�^�C���擾
	const float GetDeltaTime() { return m_deltaTime; }                               // �f���^�^�C���̎擾
	// �����蔻��
	class PhysicsWorld* const GetPhysics() { return m_physicsWorld; }                
	// �f�o�b�O�p���o���{�b�N�X
	void SetDrawDebugBox(struct OBB in_box);
	void DrawDebugBoxes();
	// �I�[�f�B�I
	class AudioManager* const GetAudio() { return m_audio; }                          // �I�[�f�B�I�̃Q�b�^�[
	// UI
	const std::vector<class UIScreen*>& GetUIStack() { return m_uiStack; }            // UI�X�^�b�N�̃Q�b�^�[
	void AddUI(class UIScreen* in_screen);
	void SwapPauseUI();                                                               // �|�[�Y��ʂ��őO�ʂɎ����Ă���
	// �t�H���g
	class Font* GetFont(const std::string& in_fileName);
	const std::string& GetFontPath() { return FONT_FILE_PATH; }
	// �e�L�X�g
	void LoadText(const std::string& in_fileName);
	const std::string& GetText(const std::string& in_key);
	// ���[�h��ʃN���X�̃Q�b�^�[
	class LoadScreen* const GetLoadScreen() { return m_load; }


private:

	GameMain();                                                              // �R���X�g���N�^

	void   Load();                                                           // �f�[�^�̃��[�h
	void   Unload();                                                         // �f�[�^�̃A�����[�h

	void Input();                                                            // ���͏���

	int   UpdateGame();                                                      // �Q�[���̍X�V����
	void UpdateActor();                                                      // �A�N�^�[�̍X�V����

	void ShowActor();                                                        // �A�N�^�[���X�g�\��(�f�o�b�O�p)

	void Draw();                                                             // �`�揈��

	// �Q�[�����[�v����ƕ`��p�̃w���p�[�֐�
	void LoopHelper();
	void DebugRenderer();

	GameState    m_state;                                                    // �Q�[���̏��(�v���C�����|�[�Y�����I����)
	
	bool m_isRunning;                                                        // �Q�[�����[�v���񂵑�����(���s����)���ǂ���
	bool m_isPauseMode;                                                      // �|�[�Y���[�h

	int          m_frame;                                                    // �t���[���v���p(�p�[�e�B�N���̐����ɗp����)
	float        m_deltaTime;                                                // 1�t���[���̌o�ߎ���(�b)
	Uint32       m_ticksCount;                                               // �~���b�P�ʂ̌o�ߎ���

	Matrix4      m_viewMatrix;

	class GameConfig* m_config;
	class Renderer* m_renderer;                                              // �����_�����O�N���X
	class SceneBase*   m_nowScene;                                           // ���݂̃V�[���̃C���X�^���X

	class CameraComponent* m_activeCamera;                                   // �A�N�e�B�u�ȃJ����

	// �����@���֘A
	class PhysicsWorld* m_physicsWorld;                                      // �����蔻��V�X�e��
	class DebugBox* m_debugBox;                                              // �f�o�b�O�p�{�b�N�X
	
    // ���y
	class AudioManager* m_audio;                                             // �I�[�f�B�I�Ǘ�

	// UI�֘A
	std::vector<class UIScreen*> m_uiStack;                                  // UI�X�^�b�N
	std::unordered_map<std::string, class Font*> m_fonts;                    // �t�H���g�}�b�v (�L�[ : ttf�t�@�C���p�X)
	std::unordered_map<std::string, std::string> m_text;                     // �e�L�X�g������ (�L�[ : �e�L�X�g������
	class PauseScreen* m_pause;                                              // �|�[�Y���
	class LoadScreen* m_load;                                                // ���[�h���

	// �A�N�^�[�z��
	std::vector<class Actor*> m_actors;                                      // �A�N�^�[�z��
	std::vector<class Actor*> m_pendingActors;                               // �A�N�^�[�ǉ������p�z��

	// �f�o�b�O�p�E�B���h�E�E�����_���[
	SDL_Window* m_debugWindow;                                               // SDL�E�B���h�E(�f�o�b�O�p)
	SDL_Renderer* m_debugRenderer;                                           // SDL�����_���[

	static const std::string FONT_FILE_PATH;                                 // �W���̃t�H���g�p�X

};

// �Q�b�^�[
#define GAME_INSTANCE GameMain::GetInstance()
// �e�N���X�|�C���^�̃Q�b�^�[define��
#define RENDERER GameMain::GetInstance().GetRenderer()
#define GAME_CONFIG GameMain::GetInstance().GetConfig()
#define AUDIO GameMain::GetInstance().GetAudio()