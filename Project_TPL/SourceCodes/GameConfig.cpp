#include "GameConfig.h"
#include <windows.h>
#include <sstream>
#include <array>

GameConfig::GameConfig()
	:m_screenWidth(-1)
	,m_screenHeight(-1)
	,m_fullScreen(true)
	,m_graphicsEnableDeferred(false)
	,m_graphicsEnableEnvMap(false)
	,m_graphicsEnableBloom(false)
{
}

GameConfig::~GameConfig()
{
}

// �R���t�B�O�̃��[�h
bool GameConfig::LoadConfig(const std::string& _iniPath)
{
	// boost�v���p�e�B�c���[��ini�t�@�C����ǂݍ���
	ptree pt;
	read_ini(_iniPath, pt);

	bool loadSuccess = true;

	// ini����f�[�^�����[�h
	loadSuccess = LoadIntValue(pt, "Display", "ScreenWidth", m_screenWidth);
	loadSuccess = LoadIntValue(pt, "Display", "ScreenHeight", m_screenHeight);

	loadSuccess = LoadBoolValue(pt, "Display", "FullScreen", m_fullScreen);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableDeferredRendering", m_graphicsEnableDeferred);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableEnvironmentMap", m_graphicsEnableEnvMap);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableBloom", m_graphicsEnableBloom);

	return loadSuccess;

	return true;
}

/// <summary>
/// ini�t�@�C�����̎w��Z�N�V�����ɂ���w��L�[�̒l(int)�����o��
/// </summary>
/// <param name="_pt"> ini�t�@�C����ǂݍ��񂾃v���p�e�B�c���[ </param>
/// <param name="_section"> ini�t�@�C�����̃Z�N�V������ ��[]�Ɋ���ꂽ������ </param>
/// <param name="_key"> ini�t�@�C�����̃L�[�� ��"  " = �� "" ���̕����� </param>
/// <param name="_val"> �ǂݍ��񂾒l���i�[����ϐ� </param>
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
