//----------------------------------------------------------------------------------+
// @file        WorldBase.h
// @brief       ワールドベースクラス(ゲームフィールドのオブジェクト管理)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/20   新規作成
//----------------------------------------------------------------------------------+
#pragma once



class WorldBase
{

public:

	WorldBase();
	virtual ~WorldBase();

	virtual bool Load() = 0;

	virtual void UpdateWorld(float _deltaTime) = 0;



protected:



	class Environment* m_environment;     // 環境クラス

	class Canvas* m_canvas;               // UIキャンバス

};