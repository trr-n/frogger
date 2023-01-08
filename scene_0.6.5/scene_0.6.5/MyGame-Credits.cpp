#include "stdafx.h"

void MyGame::Credits()
{
	while (Update()) // 注意！！
	{
		// -----------------------------------------------
		// ---------キー入力------------------------------
		// -----------------------------------------------
		if (KeySpace.down())
		{
			ChangeScene(&MyGame::Title);
		}
		// -----------------------------------------------
		// ---------画像表示------------------------------
		// -----------------------------------------------
		texture.resized(Scene::Size()).draw(0, 0);
		// テキストを画面の中心に描く | Put a text in the middle of the screen
		// -----------------------------------------------
		// ---------文字表示------------------------------
		// -----------------------------------------------
		int y60 = font60.fontSize(), y = y60 / 2;
		int y30 = font30.fontSize() + 10;
		font60(U"クレジット").drawAt(Scene::Width() / 2, y, Palette::Black);
		font30(U"プログラマー：").drawAt(Scene::Width() / 2, y += y60, Palette::Black);
		font30(U"画像素材：https://spriters-resource.com").drawAt(Scene::Width() / 2, y += y30, Palette::Black);
		font30(U"音素材：https://spriters-resource.com").drawAt(Scene::Width() / 2, y += y30, Palette::Black);
		font30(U"協力者：").drawAt(Scene::Width() / 2, y += y30, Palette::Black);

		font30(U"Push Space Key ").draw(0, 0, Palette::Black);
		// マウスカーソルに追随する半透明なハートを描く
		ColorF HeartColor = Palette::Pink;
		HeartColor.a = 0.5;
		Shape2D::Heart(100 + Periodic::Sine0_1(1s) * 40, Cursor::Pos()).draw(HeartColor);
	}
}
