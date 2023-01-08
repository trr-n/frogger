# include <Siv3D.hpp> // OpenSiv3D v0.6.5

void Main()
{
	MyGame* myGame = new MyGame();	// メモリー生成

	myGame->Run();

	delete myGame;		// メモリー開放
}
