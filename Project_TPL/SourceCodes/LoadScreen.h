#pragma once
#include "UIScreen.h"

class LoadScreen : public UIScreen
{


public:

	// ロード画面がアクティブか
	enum LOAD_STATE
	{
		ENABLE,
		DISABLE
	};

	LoadScreen();                                      // コンストラクタ
	~LoadScreen();                                     // デストラクタ

	void Initialize();

	void Update(float _deltaTime) override;                  // 更新処理

	void Draw(GLSLprogram* _shader) override;                // 描画処理


	// ロード画面の表示、非表示
	void EnableScreen();
	void DisableScreen();

	// ロード画面上で操作説明をする場合にフラグをオンにする
	void SetIsGame();

	void AddGauge();

	const int GetGaugeAllNum() { return GAUGE_ALL_NUM; }

private:


	LOAD_STATE m_state;               // ロード画面ステート

	class Texture* m_loading;

	std::vector<class Texture*> m_loadGauges;

	int m_gaugeNum;                   // ローディングゲージの最大数

	class Texture* m_bgTexture;       // ロード画面背景テクスチャ

	class Texture* m_tutorialMsg;     // ロード画面に表示するチュートリアル用テクスチャ


	bool m_isGame;                   // 今ゲームシーンかどうか(ゲームシーンなら操作説明を表示)

	std::unordered_map<std::string, std::string> m_sound;               // 効果音

	// 座標系
	Vector2 m_loadingPos;        // 「Loading」の描画位置
	Vector2 m_loadGaugePos;      // ロードゲージの描画位置

	// サイズ
	float m_loadGaugeScale;      // ロードゲージのスケール

	static const int GAUGE_ALL_NUM;
};