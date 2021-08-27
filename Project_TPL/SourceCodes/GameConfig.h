#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
using namespace boost::property_tree;

class GameConfig
{
public:

	GameConfig();
	~GameConfig();

	bool LoadConfig(const std::string& _iniPath);

	bool GetFullScreen() const { return m_fullScreen; }
	int GetScreenWidth() const { return m_screenWidth; }
	int GetScreenHeight() const { return m_screenHeight; }

	bool GetEnableDeferred() { return m_graphicsEnableDeferred; }
	bool GetEnableEnvMap() { return m_graphicsEnableEnvMap; }
	bool GetEnableBloom() { return m_graphicsEnableBloom; }

private:

	bool LoadIntValue(const ptree& _pt, const std::string& _section, const std::string& _key, int& _val);
	bool LoadBoolValue(const ptree& _pt, const std::string& _section, const std::string& _key, bool& _val);


	// スクリーン情報
    bool m_fullScreen;
	int m_screenWidth, m_screenHeight;

	// グラフィックス設定
	bool m_graphicsEnableDeferred;
	bool m_graphicsEnableEnvMap;
	bool m_graphicsEnableBloom;
	bool m_graphicsEnableNormal;
	bool m_graphicsEnableShadow;
	int  m_graphicsShadowScale;
	int  m_graphicsShadowRange;

};
