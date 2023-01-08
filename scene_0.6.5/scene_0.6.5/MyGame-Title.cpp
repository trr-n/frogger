#include "stdafx.h"

void MyGame::Title()
{

	while (Update())
	{
		// テキストを画面の中心に描く | Put a text in the middle of the screen
		font60(U"タイトル画面").drawAt(Scene::Center(), Palette::Black);

		// マウスカーソルに追随する半透明な円を描く | Draw a red transparent circle that follows the mouse cursor
		Circle{ Cursor::Pos(), 40 }.draw(ColorF{ 1, 0, 0, 0.5 });

		// もし [A] キーが押されたら | When [A] key is down
		if (KeySpace.down())
		{
			ChangeScene(&MyGame::Play);
		}

		// もし [Credits] が押されたら | When [Credits] is pushed
		if (SimpleGUI::Button(U"Credits", Vec2{ 640, 40 }))
		{
			ChangeScene(&MyGame::Credits);
		}
		font30(U"Push SpaceKey!").draw(0,0,Palette::Red);
	}

}

