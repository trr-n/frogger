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
	void(MyGame::*beforeScene)() = nullptr;// ポインタ変数 beforeSceneの定義 [  void() *beforeScene = nullptr;  ]// イメージ
	void(MyGame::*scene)() = &MyGame::Play;// [  void() *scene = this->Play;  ]// イメージ

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
	void GameOver();
	void Credits();	// 開発者名、協力者名、素材提供元など
//	void Ranking();
//	void Ending();

	// 通常のフォントを作成 MyGameクラス内ならどこでも使える（例：Main()内では、MyGameクラス外なので使えない）
	const Font font30{ 30 };
	const Font font60{ 60 };

	// 画像ファイルからテクスチャを作成 | Create a texture from an image file
	const Texture texture{ U"example/windmill.png" };
	


};

