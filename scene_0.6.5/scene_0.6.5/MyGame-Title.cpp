#include "stdafx.h"

void MyGame::Title()
{
	while (Update()) {
		auto width = Scene::Width() / 2, height = Scene::Height() / 2;

		if (SimpleGUI::Button(U"Credit", Vec2{ 697, 5 }))
			ChangeScene(&MyGame::Credits);

		if (SimpleGUI::ButtonAt(U"Game Start", Vec2{ width, height + Tile }))
			ChangeScene(&MyGame::Play);

		if (SimpleGUI::ButtonAt(U"Exit", Vec2{ width, height + Tile * 2 }))
			System::Exit();

		frogger.drawAt(width, height);
	}
}

