#include "stdafx.h"

void MyGame::Run()
{
	SetScreen();

	SetScene(&MyGame::Title);// かなりややこしい記述だが仕方ないC++仕様  C#の同等機能ではこんなにややこしくない
	while (scene != nullptr)
	{
		(this->*scene)();
	}
}

/// @brief シーンを設定する
/// @param $scene 変えたいシーン　例：Playなど
void MyGame::SetScene(void(MyGame::* $scene)())
{
	scene = beforeScene = $scene;
}

/// @brief シーンを変える
/// @param $scene 変えたいシーン　例：Playなど
void MyGame::ChangeScene(void(MyGame::* $scene)())
{
	beforeScene = scene;	// 変える前のシーン
	scene = $scene;	// 変えるシーン
}

/// @brief 画面更新と現在のシーンの終了チェック
/// @return true:シーンそのまま  false:このシーンを終了する場合
bool MyGame::Update()
{
	if (!System::Update())
	{
		scene = nullptr;
	}
	bool UpdateLoop = (scene == beforeScene);
	if (! UpdateLoop)
	{
		beforeScene = scene;	// 次回のループを回すために同じにする
	}
	return UpdateLoop;	// ここが false だと while(Update)ループから出る
}

void MyGame::Exit()
{
	scene = nullptr;
}

/// @brief ウィンドウ画面の初期化
void MyGame::SetScreen()
{
	// 背景の色を設定 | Set background color
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });
}
