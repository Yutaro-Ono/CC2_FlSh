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

	void Update(float _deltaTime) override;          // �X�V����

	void Draw(GLSLprogram* _shader) override;                // �`�揈��


	// ���[�h��ʂ̕\���A��\��
	void EnableScreen();
	void DisableScreen();

	// ���[�h��ʏ�ő������������ꍇ�Ƀt���O���I���ɂ���
	void SetIsGame();

	void AddGauge();


private:



	class Texture* m_loading;

	std::vector<class Texture*> m_loadGauges;

	int m_gaugeNum;

	class Texture* m_bgTexture;

	class Texture* m_tutorialMsg;

	LOAD_STATE m_state;

	bool m_isGame;                   // ���Q�[���V�[�����ǂ���(�Q�[���V�[���Ȃ瑀�������\��)

	std::unordered_map<std::string, std::string> m_sound;               // ���ʉ�

	Vector2 m_loadingPos;

	Vector2 m_loadGaugePos;

	static const int GAUGE_NUM;
};