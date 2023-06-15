#pragma once
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
	void Clear();
	// void GameOver();
	// void Ranking();

	// 通常のフォントを作成 MyGameクラス内ならどこでも使える（例：Main()内では、MyGameクラス外なので使えない）
	const Font font30{ 30 };
	const Font font60{ 60 };
	const Font fontBold{ 50, Typeface::Regular, FontStyle::Bold };

	const String BubbleManTTF_path = U"fonts/BubbleMan.ttf";

	const Texture halfway{ U"stageObjects/footing.png" }; // ピンクの足場
	const Texture goal{ U"stageObjects/goal.png" }; // ゴール
	const Texture nest{ U"stageObjects/nest.png" }; // ゴール2
	const Texture car0{ U"stageObjects/car1.png" }; // 車
	const Texture car1{ U"stageObjects/car2.png" };
	const Texture car2{ U"stageObjects/car3.png" };
	const Texture car3{ U"stageObjects/car4.png" };
	const Texture car4{ U"stageObjects/car5.png" };
	const Texture turtle{ U"stageObjects/turtle.png" }; // 亀 アニメーション
	const Texture turtleAnims{ U"stageObjects/turtlePatterns.png" }; // 亀 アニメーション
	const Texture log{ U"stageObjects/log.png" }; // 丸太
	const Texture crocodile{ U"stageObjects/crocodile.png" }; // ワニ
	const Texture deadPattern{ U"frogAnimations/death_pattern.png" };
	const Texture plunkPattern{ U"frogAnimations/plunk_pattern.png" };

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
	const Audio bgm{ U"sounds/musics/frogger-music.mp3" };
	const Audio bgm1{ U"sounds/musics/y2mate.com - Clarx  Zig Zag NCS Release.mp3" };

	// かえるアニメーション
	const Texture frogsForward{ U"frogAnimations/forward.png" };
	const Texture frogsBack{ U"frogAnimations/back.png" };
	const Texture frogsLeft{ U"frogAnimations/left.png" };
	const Texture frogsRight{ U"frogAnimations/right.png" };

	const Texture frogSitting{ U"frogAnimations/frog_repository.png" };
	// "D:\chickenen\prgs\Scene_065\scene_0.6.5\scene_0.6.5\App\frogAnimations\frog_repository.png"

	// シーンサイズxy, タイルサイズ
	const int SceneX = 896, SceneY = 1152, Tile = 64;
};

