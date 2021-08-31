//-----------------------------------------------------------------------+
//
// Canvasクラス (UIクラスがワールドクラスを参照できるようにする)
//
//-----------------------------------------------------------------------+
#pragma once
#include <vector>

class Canvas
{
public:


	Canvas(class WorldBase* in_world);     // コンストラクタ
	~Canvas();                             // デストラクタ

	virtual bool Load() = 0;

	virtual void Update(float _deltaTime);

	void AddUI(class UIScreen* in_ui) { m_interfaces.push_back(in_ui); }


private:


	class WorldBase* m_world;                       // ゲームワールド

	std::vector<class UIScreen*> m_interfaces;      // インターフェース

};