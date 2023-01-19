﻿#pragma once
class MyGame
{
public:

	MyGame() {}
	void Run();

private:
	// ----------------シーン管理用のもの------------------------------------------------------------
	// かなりややこしいが仕方ないC++仕様  C#の同等機能ではこんなにややこしくない
	// 関数のポインタの変数定義
	void(MyGame::* beforeScene)() = nullptr;// ポインタ変数 beforeSceneの定義 [  void() *beforeScene = nullptr;  ]// イメージ
	void(MyGame::* scene)() = &MyGame::Play;// [  void() *scene = this->Play;  ]// イメージ

	// 情報源  http://www7b.biglobe.ne.jp/~robe/cpphtml/html03/cpp03057.html

	void SetScreen();
	void ChangeScene(void(MyGame::* $scene)());
	void SetScene(void(MyGame::* $scene)());
	bool Update();
	/// @brief アプリ終了
	void Exit();

	//-------------ゲーム実装部分------------------------------------------

	void Title();
	void Play();
	void Credits();
	// void GameOver();
	// void Ranking();
	// void Ending();

	// 通常のフォントを作成 MyGameクラス内ならどこでも使える（例：Main()内では、MyGameクラス外なので使えない）
	const Font font30{ 30 };
	const Font font60{ 60 };

	const Texture footing{ U"stageObjects/footing.png" }; // ピンクの足場
	const Texture goal{ U"stageObjects/goal.png" }; // ゴール
	const Texture car1{ U"stageObjects/car1.png" }; // 車
	const Texture car2{ U"stageObjects/car2.png" };
	const Texture car3{ U"stageObjects/car3.png" };
	const Texture car4{ U"stageObjects/car4.png" };
	const Texture car5{ U"stageObjects/car5.png" };
	const Texture turtle{ U"stageObjects/turtle.png" }; // 亀 アニメーション
	const Texture log{ U"stageObjects/log.png" }; // 丸太
	const Texture crocodile{ U"stageObjects/crocodile.png" }; // ワニ

	const Texture frogger{ U"char/frogger.png" };
	const Texture f{ U"char/f.png" };
	const Texture r{ U"char/r.png" };
	const Texture o{ U"char/o.png" };
	const Texture g{ U"char/g.png" };
	const Texture e{ U"char/e.png" };

	const Texture credit{ U"credits/credit.png" };
	const Texture odc{ U"credits/odc.png" };
	const Texture s3d{ U"credits/siv3d.png" };

	const Audio squash{ U"sounds/squash.wav" };
	const Audio jumpSound{ U"sounds/jump.mp3" };
	const Audio plunkSound{ U"sounds/plunk.wav" };
	const Audio timeupSound{ U"sounds/timeup.wav" };

	// かえるアニメーション
	const Texture frogsForward{ U"frogAnimations/forward.png" };
	const Texture frogsBack{ U"frogAnimations/back.png" };
	const Texture frogsLeft{ U"frogAnimations/left.png" };
	const Texture frogsRight{ U"frogAnimations/right.png" };

	const Texture frogRepo{ U"frog_repository.png" };

	// シーンサイズxy, タイルサイズ
	const int SceneX = 896, SceneY = 1152, Tile = 64;
};

