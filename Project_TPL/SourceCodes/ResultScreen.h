//-----------------------------------------------------------------------+
// ���U���g�V�[���pUI�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"


class ResultScreen : public UIScreen
{


public:

	ResultScreen(class ResultScene* _targetScene, int _score, float _bestSpeed);
	~ResultScreen();


	void Update(float _deltaTime) override;

	void Draw(GLSLprogram* _shader) override;

	int GetDispScore() { return m_dispScore; }


	void SetRank(const int& _rank) { m_rank = _rank; }

	void SetScore(const int& _num, const int& _score);                    // �����L���O���̃Z�b�^�[


private:

	class ResultScene* m_targetScene;

	class Texture* m_resultTex[2];
	class Texture* m_scoreTex[2];
	class Texture* m_restartButton[2];
	class Texture* m_quitButton[2];

	class Texture* m_rankingScore[10];          // �����L���O���Ƃ̃X�R�A�e�N�X�`��

	// ����\���pUI
	class Texture* m_xpadSkipTex;                   // Xbox�R���g���[��
	class Texture* m_keyboardSkipTex;               // �L�[�{�[�h

	class Texture* m_xpadTutorialTex;               // Xbox�R���g���[��
	class Texture* m_keyboardTutorialTex;           // �L�[�{�[�h

	int m_score;

	int m_rank;

	int m_dispScore;

	float m_counter;

	int m_fontSize;

	Vector2 m_resultTexPos;
};