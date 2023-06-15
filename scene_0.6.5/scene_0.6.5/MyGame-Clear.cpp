# include <stdio.h>
//#include <boost\format.hpp>
//# include <string>

void MyGame::Clear()
{
	Font scoresFont = General::FontSetting(60, BubbleManTTF_path);
	while (Update())
	{
		Scene::SetBackground(Palette::Black);
		scoresFont(U"Your Record\nScore: {}\nTime: {}"_fmt(General::FinalPoint, General::FinalTime)).drawAt(Scene::Center());
	}
}
