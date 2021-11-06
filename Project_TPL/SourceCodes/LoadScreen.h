#pragma once
#include "UIScreen.h"

class LoadScreen : public UIScreen
{


public:

	// ���[�h��ʂ��A�N�e�B�u��
	enum LOAD_STATE
	{
		ENABLE,
		DISABLE
	};

	LoadScreen();                                      // �R���X�g���N�^
	~LoadScreen();                                     // �f�X�g���N�^

	void Initialize();

	void Update(float _deltaTime) override;                  // �X�V����

	void Draw(GLSLprogram* _shader) override;                // �`�揈��


	// ���[�h��ʂ̕\���A��\��
	void EnableScreen();
	void DisableScreen();

	// ���[�h��ʏ�ő������������ꍇ�Ƀt���O���I���ɂ���
	void SetIsGame();

	void AddGauge();

	const int GetGaugeAllNum() { return GAUGE_ALL_NUM; }

private:


	LOAD_STATE m_state;               // ���[�h��ʃX�e�[�g

	class Texture* m_loading;

	std::vector<class Texture*> m_loadGauges;

	int m_gaugeNum;                   // ���[�f�B���O�Q�[�W�̍ő吔

	class Texture* m_bgTexture;       // ���[�h��ʔw�i�e�N�X�`��

	class Texture* m_tutorialMsg;     // ���[�h��ʂɕ\������`���[�g���A���p�e�N�X�`��


	bool m_isGame;                   // ���Q�[���V�[�����ǂ���(�Q�[���V�[���Ȃ瑀�������\��)

	std::unordered_map<std::string, std::string> m_sound;               // ���ʉ�

	// ���W�n
	Vector2 m_loadingPos;        // �uLoading�v�̕`��ʒu
	Vector2 m_loadGaugePos;      // ���[�h�Q�[�W�̕`��ʒu

	// �T�C�Y
	float m_loadGaugeScale;      // ���[�h�Q�[�W�̃X�P�[��

	static const int GAUGE_ALL_NUM;
};