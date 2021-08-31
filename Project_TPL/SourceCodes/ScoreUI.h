//-----------------------------------------------------------------------+
// スコアUIクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"

class ScoreUI : public UIScreen
{


public:

	ScoreUI(class CanvasGameScene* _canvas);                   // コンストラクタ
	~ScoreUI();                                          // デストラクタ

	void Update(float _deltaTime) override;            // 更新処理

	void Draw(GLSLprogram* _shader) override;                  // 描画処理

	//-------------------------------------------------+
	// Setter/Getter
	//-------------------------------------------------+
	void AddScore();    // スコアの加算処理
	void SubScore();    // スコアの減算処理
	// 加算分スコアのセットとそれに伴うテクスチャ生成
	void SetAddScore(int in_score);

	// 現在のスコア (加算分含む)
	int GetScore() { return m_score + m_addScore; }

private:

	class CanvasGameScene* m_canvas;

	class Texture* m_scoreTex[2];
	class Texture* m_addScoreTex[2];
	class Texture* m_subScoreTex[2];

	Vector2 m_scoreTexPos;
	Vector2 m_addScoreTexPos;
	Vector2 m_subScoreTexPos;

	int m_score;      // 毎フレームのスコア
	int m_addScore;      // スコアの追加分格納用

	bool m_drawSubScore;

	int m_fontSize;

	// スコア減算時に用いるカウンター
	bool m_startSub;
	unsigned int m_time;
	unsigned int m_coolTime;
};