//----------------------------------------------------------------------------------+
// @file        GameMain.h
// @brief       �Q�[�����[�v�Ɋւ��C���X�^���X�𐶐��E�ێ�
//              �X�V�������s��
// @note        �V���O���g�������Ainclude����΂ǂ�����ł��A�N�Z�X�\
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/21   �V�K�쐬
//
//----------------------------------------------------------------------------------+
#pragma once
// �C���N���[�h�t�@�C��
#include <unordered_map>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_types.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Math.h"
#include "GameConfig.h"
#include "Renderer.h"
#include "Input.h"
#include "Mouse.h"
#include "InputController.h"

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

	// �V���O���g��(�ÓI�C���X�^���X�̃Q�b�^�[)
	static GameMain& GetInstance()
	{
		static GameMain game;
		return game;
	}

	~GameMain();                                                                    // �f�X�g���N�^

	bool   Initialize();                                                            // �e�평��������
	void   Delete();                                                                // �e��������

	void   RunLoop();                                                               // �Q�[���̃��C�����[�v
	void   CloseGame();                                                             // �Q�[���̏I������

	void   SetFirstScene(class SceneBase* _scene);                                // �J�n�V�[���̃Z�b�g

	void   SetShutDown() { m_isRunning = false; }                                     // �V���b�g�_�E������

	void   AddActor(class Actor* _actor);                                           // �A�N�^�[�̒ǉ�
	void   RemoveActor(class Actor* _actor);                                        // �A�N�^�[�̍폜
	void   DeadAllActor();                                                            // �A�N�^�[�S�폜


    //--------------------------------------------------------------------//
	// Getter/Setter
	//-------------------------------------------------------------------//
	class GameConfig* GetConfig() const { return m_config; }                         // �Q�[���ݒ�N���X�̃Q�b�^�[
	class Renderer* GetRenderer() const { return m_renderer; }                       // �����_���[�擾
	class Debugger* GetDebugger() const { return m_debugger; }                       // �f�o�b�K�\�擾
	class AudioManager* const GetAudio() { return m_audio; }                         // �I�[�f�B�I�̃Q�b�^�[
	class PhysicsWorld* const GetPhysics() { return m_physicsWorld; }                // �����蔻��N���X�̃Q�b�^�[
	class LoadScreen* const GetLoadScreen() { return m_loadScreen; }                 // ���[�h�X�N���[���N���X�̃Q�b�^�[
	class ActorPool* const GetActorPool() { return m_actorPool; }

	SDL_Renderer* GetSDLRenderer();                                                  // 2D�pSDL�����_���[�̎擾
	// �J����
	void SetCamera(class Camera* _camera);                                         // �J�������V�X�e���ɓo�^
	void SetCamera(class CameraComponent* _camera);                                // �J�������V�X�e���ɓo�^
	class CameraComponent* GetCamera() { return m_activeCamera; }                    // �A�N�e�B�u�ȃJ�����|�C���^�̎擾

	void InActiveCamera(class Camera* _activeCam);                                 // �J�����̓o�^������
	void InActiveCamera(class CameraComponent* _activeCam);                        // �J�����̓o�^������
	// �s��擾
	const Matrix4& GetViewMatrix() { return m_viewMatrix; };                         // �r���[�s��̃Q�b�^�[
	const Vector3& GetViewVector();                                                  // �r���[���W�̃Q�b�^�[
	// �t���[���v��
	const int GetFrame() { return m_frame; }                                         // �t���[���̃Q�b�^�[
	// �f���^�^�C���擾
	const float GetDeltaTime() { return m_deltaTime; }                               // �f���^�^�C���̎擾
	// �f�o�b�O�p���o���{�b�N�X
	void SetDrawDebugBox(struct OBB _box);
	void DrawDebugBoxes();
	// UI
	const std::vector<class UIScreen*>& GetUIStack() { return m_uiStack; }            // UI�X�^�b�N�̃Q�b�^�[
	void AddUI(class UIScreen* _screen);
	void SwapPauseUI();                                                               // �|�[�Y��ʂ��őO�ʂɎ����Ă���
	// �t�H���g
	class Font* GetFont(const std::string& _fileName);
	const std::string& GetFontPath() { return FONT_FILE_PATH; }
	// �e�L�X�g
	void LoadText(const std::string& _fileName);
	const std::string& GetText(const std::string& _key);

private:

	GameMain();                                                              // �R���X�g���N�^

	void Input();                                                            // ���͏���

	int   UpdateGame();                                                      // �Q�[���̍X�V����
	void UpdateActor();                                                      // �A�N�^�[�̍X�V����                                                   // �A�N�^�[���X�g�\��(�f�o�b�O�p)

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
	class SceneBase*   m_nowScene;                                           // ���݂̃V�[��

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
	class PauseScreen* m_pauseScreen;                                        // �|�[�Y���
	class LoadScreen* m_loadScreen;                                          // ���[�h���

	// �e��I�u�W�F�N�g�v�[���Q
	class ActorPool* m_actorPool;
	class MeshPool* m_meshPool;
	class TexturePool* m_texturePool;

	// �f�o�b�O�N���X
	class Debugger* m_debugger;

	static const std::string FONT_FILE_PATH;                                 // �W���̃t�H���g�p�X

};

// �Q�b�^�[
#define GAME_INSTANCE GameMain::GetInstance()
// �e�N���X�|�C���^�̃Q�b�^�[define��
#define RENDERER GameMain::GetInstance().GetRenderer()
#define GAME_CONFIG GameMain::GetInstance().GetConfig()
#define DEBUGGER GameMain::GetInstance().GetDebugger()
#define AUDIO GameMain::GetInstance().GetAudio()
#define ACTOR_POOL GameMain::GetInstance().GetActorPool()