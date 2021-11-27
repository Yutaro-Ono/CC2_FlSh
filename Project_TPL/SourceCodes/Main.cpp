//-----------------------------------------------------------------------+
// メインループ.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
// インクルードファイル
#include "GameMain.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "TutorialScene.h"
#include "ResultScene.h"
#include "TitleSceneTPL.h"
#include "GameSceneTPL.h"
#include <iostream>
#include <string>

// ランチャー(Launcher.exe)呼び出し関数 
// pathにはLauncher.exeへの相対パスへの文字列を入れる
void runLauncher(const char* path)
{
	#ifndef _DEBUG
	char fullPathexe[512];
#pragma warning (disable:4996)
	sprintf(fullPathexe, "%s%s", path, "Launcher.exe");

	// プロセス起動準備
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);

	CreateProcess((LPCWSTR)fullPathexe, (LPWSTR)"", NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, (LPCWSTR)path, &si, &pi);

	#endif
}


// メインループ
int main(int argc, char** argv)
{

	// ゲームの初期化処理(画面の横幅, 画面の縦幅, フルスクリーン:true false)
	// 初期化に失敗したらfalseを返す
	bool success = GAME_INSTANCE.Initialize();

	if (success)
	{
		// 最初のシーンのセット
		//GAME_INSTANCE.SetFirstScene(new GameSceneTPL());
		GAME_INSTANCE.SetFirstScene(new TitleSceneTPL());

		// ゲームループ
		GAME_INSTANCE.RunLoop();

	}

	// ゲームの終了処理
	GAME_INSTANCE.CloseGame();

	// 終了時ランチャー呼び出し
	runLauncher("../../");

	return 0;

}