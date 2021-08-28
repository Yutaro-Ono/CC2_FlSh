#include "GameConfig.h"
#include <windows.h>
#include <string>
#include <sstream>
#include <array>

GameConfig::GameConfig()
	:m_screenWidth(-1)
	,m_screenHeight(-1)
	,m_fullScreen(true)
	,m_graphicsEnableDeferred(false)
	,m_graphicsEnableEnvMap(false)
	,m_graphicsEnableBloom(false)
	,m_graphicsEnableNormal(false)
	,m_graphicsEnableShadow(false)
	,m_graphicsShadowScale(2048)
	,m_graphicsShadowRange(7000)
{
}

GameConfig::~GameConfig()
{
}

// コンフィグのロード
bool GameConfig::LoadConfig(const std::string& _iniPath)
{
	// boostプロパティツリーにiniファイルを読み込み
	ptree pt;
	read_ini(_iniPath, pt);

	bool loadSuccess = true;

	// iniからデータをロード
	loadSuccess = LoadIntValue(pt, "Display", "ScreenWidth", m_screenWidth);
	loadSuccess = LoadIntValue(pt, "Display", "ScreenHeight", m_screenHeight);
	// グラフィック設定
	loadSuccess = LoadBoolValue(pt, "Display", "FullScreen", m_fullScreen);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableDeferredRendering", m_graphicsEnableDeferred);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableEnvironmentMap", m_graphicsEnableEnvMap);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableBloom", m_graphicsEnableBloom);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableNormalMap", m_graphicsEnableNormal);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableShadow", m_graphicsEnableShadow);
	loadSuccess = LoadIntValue(pt, "Graphics", "ShadowScale", m_graphicsShadowScale);
	loadSuccess = LoadIntValue(pt, "Graphics", "ShadowRange", m_graphicsShadowRange);


	return loadSuccess;

	return true;
}

/// <summary>
/// iniファイル内の指定セクションにある指定キーの値(int)を取り出す
/// </summary>
/// <param name="_pt"> iniファイルを読み込んだプロパティツリー </param>
/// <param name="_section"> iniファイル内のセクション名 ※[]に括られた文字列 </param>
/// <param name="_key"> iniファイル内のキー名 ※"  " = の "" 内の文字列 </param>
/// <param name="_val"> 読み込んだ値を格納する変数 </param>
/// <returns></returns>
bool GameConfig::LoadIntValue(const ptree& _pt, const std::string& _section, const std::string& _key, int& _val)
{
	std::string sectionKey = _section + "." + _key;
	_val = _pt.get<int>(sectionKey);
	return _val == -1 ? false : true;
}

bool GameConfig::LoadBoolValue(const ptree& _pt, const std::string& _section, const std::string& _key, bool& _val)
{
	std::string sectionKey = _section + "." + _key;
	_val = _pt.get<bool>(sectionKey);
	return _val == -1 ? false : true;
}
