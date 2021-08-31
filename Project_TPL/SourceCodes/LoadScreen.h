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

	void Update(float _deltaTime) override;          // 更新処理

	void Draw(GLSLprogram* _shader) override;                // 描画処理


	// ロード画面の表示、非表示
	void EnableScreen();
	void DisableScreen();

	// ロード画面上で操作説明をする場合にフラグをオンにする
	void SetIsGame();

	void AddGauge();


private:



	class Texture* m_loading;

	std::vector<class Texture*> m_loadGauges;

	int m_gaugeNum;

	class Texture* m_bgTexture;

	class Texture* m_tutorialMsg;

	LOAD_STATE m_state;

	bool m_isGame;                   // 今ゲームシーンかどうか(ゲームシーンなら操作説明を表示)

	std::unordered_map<std::string, std::string> m_sound;               // 効果音

	Vector2 m_loadingPos;

	Vector2 m_loadGaugePos;

	static const int GAUGE_NUM;
};