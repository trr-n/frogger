# include <Siv3D.hpp> // OpenSiv3D v0.6.5

void Main()
{
	// メモリー生成
	MyGame* myGame = new MyGame();

	myGame->Run();

	// メモリー開放
	delete myGame;
}
