//-----------------------------------------------------------------------+
// リザルトシーン用UIクラス.
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

	void SetScore(const int& _num, const int& _score);                    // ランキング情報のセッター


private:

	class ResultScene* m_targetScene;

	class Texture* m_resultTex[2];
	class Texture* m_scoreTex[2];
	class Texture* m_restartButton[2];
	class Texture* m_quitButton[2];

	class Texture* m_rankingScore[10];          // ランキングごとのスコアテクスチャ

	// 操作表示用UI
	class Texture* m_xpadSkipTex;                   // Xboxコントローラ
	class Texture* m_keyboardSkipTex;               // キーボード

	class Texture* m_xpadTutorialTex;               // Xboxコントローラ
	class Texture* m_keyboardTutorialTex;           // キーボード

	int m_score;

	int m_rank;

	int m_dispScore;

	float m_counter;

	int m_fontSize;

	Vector2 m_resultTexPos;
};