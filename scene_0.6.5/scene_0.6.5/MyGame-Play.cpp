#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <math.h>

//using V2 = std::map<std::string, Vec2>;
using str = std::string;

void MyGame::Play()
{
	if (SimpleGUI::ButtonAt(U"Title", Vec2(0, Scene::Height())))
		ChangeScene(&MyGame::Title);
	Scene::Resize(SceneX, SceneY);
	Scene::SetBackground(Color(0, 0, 26));
	TextureRegion frogAnims[8] = {
		frogsForward(0, 0, Tile * 1, Tile),
		frogsForward(Tile, 0, Tile * 2, Tile * 2),
		frogsBack(0, 0, Tile * 1, Tile),
		frogsBack(Tile,	0, Tile * 2, Tile * 2),
		frogsLeft(0, 0, Tile * 1, Tile),
		frogsLeft(Tile,	0, Tile * 2, Tile * 2),
		frogsRight(Tile, 0, Tile * 2, Tile * 2),
		frogsRight(0, 0, Tile * 1, Tile)
	};
	const Vec2 FrogSpawn(448, 1024);
	const Vec2 frogOut(2048, 2048);
	Vec2 frogPos(FrogSpawn);
	Vec2 frogVel(0, 0);
	Vec2 nestPos(0, 220);

	/// @brief 1: left up, left bottom, right up, right bottom
	SpawnPosition spawnPos(Point(-128, 512), Point(-128, 896), Point(960, 576), Point(900, 960));
	const int sceneWidth = Scene::Width() - Tile, sceneHeight = Scene::Height() - Tile * 2;
	Speeds speeds(80 / 60, 70 / 60, 80 / 60, 180 / 60, 65 / 60, 160 / 60, 120 / 60);

	Array<GameObject> cars, logs, logs1;
	int carLanes[] = { 0, 0, 2, 2, 4 };
	for (int i = 0; i < 4; i++)
	{
		auto add_space = [&]() { return i * Random(200, 400); };
		cars << GameObject(car0, spawnPos.rightBottom - Vec2(add_space(), Tile * 0), -speeds.car0);
		cars << GameObject(car1, spawnPos.leftBottom + Vec2(add_space(), Tile * 0), speeds.car1);
		cars << GameObject(car2, spawnPos.rightBottom - Vec2(add_space(), Tile * 2), -speeds.car2);
		cars << GameObject(car3, spawnPos.leftBottom + Vec2(add_space(), -Tile * 2), speeds.car3);
		cars << GameObject(car4, spawnPos.rightBottom - Vec2(add_space(), Tile * 4), -speeds.car4);
	}
	int logLanes[] = { 0, 2, 3 };
	int logMin = 400, logMax = 700;
	for (auto j = 0; j < 4; j++)
	{
		auto add_space = [&]() { return j * Random(300, 500); };
		logs << GameObject(log, spawnPos.leftUp - Vec2(add_space(), Tile * logLanes[0]) + Vec2(-100, 0), speeds.log);
		logs << GameObject(log, spawnPos.leftUp - Vec2(add_space(), Tile * logLanes[1]) + Vec2(-10, 0), speeds.log);
		logs << GameObject(log, spawnPos.leftUp - Vec2(add_space(), Tile * logLanes[2]), speeds.log);
	}

	Array<Turtle> turtles;
	int turtleLanes[] = { 0, 2 };
	for (auto i = 0; i < 3; i++)
	{
		for (auto j = 0; j < 3; j++)
		{
			turtles << Turtle(turtleAnims, spawnPos.rightUp - Vec2(Tile * j, Tile * turtleLanes[0]), -speeds.turtle);
			turtles << Turtle(turtleAnims, spawnPos.rightUp - Vec2(Tile * j, Tile * turtleLanes[1]), -speeds.turtle);
		}
	}

	Stopwatch sw;
	Stopwatch respawnTimer;

	const int Up = 0, Back = 2, Left = 4, Right = 6;
	int frogPattern = Up;
	int moving = 1;
	int count = 0;
	int breakTime = 2;
	bool isDead = false;
	bool isCtrl = true;
	float bgmVolume = 0;
	bgm1.play();
	bgm1.setVolume(bgmVolume);
	Stopwatch nesting;
	bool hitLogOrTurtle = false;
	const int Y = 246;
	FrogNest frogNests[5] = {
		FrogNest(33, Y, Tile, Tile, frogSitting),
		FrogNest(223, Y, Tile, Tile, frogSitting),
		FrogNest(414, Y, Tile, Tile, frogSitting),
		FrogNest(606, Y, Tile, Tile, frogSitting),
		FrogNest(803, Y, Tile, Tile, frogSitting)
	};
	Score score = { 0, 100, 334, -10 };
	Stopwatch timer;
	timer.start();
	int playTime = 0;
	std::vector<bool> selves;
	bool isHitFrame = false;

	while (Update())
	{
		ClearPrint();
		playTime = floor(timer.sF());

		// ---------------移動-------------------
		auto Jumping = [&](int _pattern, int _x, int _y)
		{
			frogPattern = _pattern + 1;
			frogVel.set(_x, _y);
			jumpSound.playOneShot();
			score.Add(score.jump);
			count = 0;
		};
		if (squash.posSec() >= squash.lengthSec() - 0.1)
		{
			FrogNest::intoNest = false;
			frogVel = Vec2(0, 0);
		}

		while (!FrogNest::intoNest)
		{
			if (frogPattern % 2 == 0 && isCtrl)
			{
				auto pad = Controller(0);
				pad.Update(Type::Down);
				if (pad.forward)
					Jumping(Up, 0, -moving);
				else if (pad.back)
					Jumping(Back, 0, moving);
				else if (pad.left)
					Jumping(Left, -moving, 0);
				else if (pad.right)
					Jumping(Right, moving, 0);
			}
			else
			{
				auto velocity = frogVel;
				switch (count)
				{
				case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:
					velocity *= 8;
					break;
				default:
					throw std::exception();
					break;
				}
				frogPos += velocity;
				count++;
				if (count > 7)
					frogPattern -= 1;
			}
			break;
		}

		// 画面外にでないように移動可能な範囲を限定
		frogPos = Vec2(std::clamp((int)frogPos.x, 0, sceneWidth), std::clamp((int)frogPos.y, 220, sceneHeight));
		Rect frogCol(frogPos.asPoint() + Point(16, 16), (frogsForward.width() / 2) - Tile / 2, frogsForward.height() - Tile / 2);
#if _DEBUG
		frogCol.draw(ColorF(Palette::Cyan, 0.25));
#endif

#pragma region 当たり判定
		// くるまの移動と当たり判定
		for (auto& i : cars)
		{
			i.Update();
			Rect obsCol(i.position.asPoint(), i.texture.width(), i.texture.height());
			if (obsCol.intersects(frogCol) && !isDead)
			{
				squash.playOneShot();
				isDead = true;
				if (isDead)
					respawnTimer.start();
			}
			if (i.position.x < -300)
				i.position.x = spawnPos.rightBottom.x;
			if (i.position.x > 1200)
				i.position.x = spawnPos.leftBottom.x;
		}

		// 水没
		Rect plunkArea(0, 325, 900, 300);
		Stopwatch plunkTimer;
		plunkArea.draw(ColorF(Palette::Skyblue, .25));
		/*bool ona;
		if (ona = frogCol.intersects(plunkArea) && !(count <= 6 && count >= 2) && !hitLogOrTurtle)
		{
			frogPos = frogOut;
			plunkTimer.start();
			plunkSound.playOneShot();
			score.Add(score.hit);
		}

		if (plunkTimer.sF() >= 2)
		{
			frogPos = FrogSpawn;
			plunkTimer.reset();
		}*/

		// 丸太 -------------
		hitLogOrTurtle = false;
		for (auto& log0 : logs)
		{
			Rect logCol(log0.position.asPoint(), log0.texture.width(), log0.texture.height());
			if (logCol.intersects(frogCol))
			{
				hitLogOrTurtle = true;
				log0.playerPosPtr = &frogPos;
				log0.Riding(&frogPos);
				*(log0.playerPosPtr) += Vec2(2, 0);
			}
			log0.Update();
			if (log0.position.x > 1200)
				log0.position.x = spawnPos.leftBottom.x;
		}

		for (auto& turtle0 : turtles)
		{
			Rect turtleCol(turtle0.position.asPoint(), Tile, Tile);
			if (turtleCol.intersects(frogCol) && !turtle0.IsPlunking())
			{
				hitLogOrTurtle = true;
				turtle0.playerPosPtr = &frogPos;
				turtle0.Riding(&frogPos);
				*(turtle0.playerPosPtr) -= Vec2(0.25, 0);
			}
			turtle0.Update();
			if (turtle0.position.x < -300)
				turtle0.position.x = spawnPos.rightBottom.x;
		}

		for (auto pinkie = 0; pinkie < Scene::Width(); pinkie += Tile)
		{
			Vec2 f0(pinkie, sceneHeight), f1(pinkie, sceneHeight / 2);
			halfway.draw(pinkie, sceneHeight);
			halfway.draw(pinkie, 640);
		}

		for (auto& nest : frogNests)
			if (!isHitFrame)
				nest.HitCheck(frogCol);

		if (FrogNest::intoNest && !squash.isPlaying())
		{
			score.Add(score.nest);
			squash.play();
		}

#pragma endregion

#pragma region 描画

		// ゴール -------------

		// 一番左の巣の当たり判定の座標、間隔
		const int goalx = 33, goaly = 250, nests = 193;
		for (int nst = 0; nst < 5; nst++)
		{
			Rect nestsCol[5] = { { goalx + nests * nst,goaly, 60, 30 } };
			for (auto& j : nestsCol)
				j.draw(ColorF(Palette::Red, .5));
			if (FrogNest::intoNest)
				frogPos = FrogSpawn;
		}

		for (auto& c : cars) c.Draw();
		for (auto& l : logs) l.Draw();
		for (auto& l : logs1) l.Draw();
		for (auto& t : turtles) t.Draw();
		nest.draw(nestPos);

		if (sw.isRunning() && sw.sF() < 3)
			Rect(frogPos.asPoint(), Tile, Tile).draw(Palette::Red);
		else
			frogAnims[frogPattern].draw(frogPos);

		if (isDead)
		{
			moving = 0;
			int pattern = (int)(respawnTimer.sF() / (breakTime / 7.0));
			auto w = deadPattern.width() / 7;
			deadPattern(pattern * w, 0, w, Tile).draw(frogPos);

			if (respawnTimer.sF() >= breakTime)
			{
				frogPos = FrogSpawn;
				isDead = false;
				moving = 1;
				respawnTimer.reset();
			}
		}
		selves = {
			frogNests[0].intoNestSelf,
			frogNests[1].intoNestSelf,
			frogNests[2].intoNestSelf,
			frogNests[3].intoNestSelf,
			frogNests[4].intoNestSelf,
		};

		// クリア判定
		if (std::all_of(selves.begin(), selves.end(), [](bool b) { return b; }))
		{
			timer.pause();
			General::SetFinalScores(score.current, timer.sF());
			ChangeScene(&MyGame::Clear);
		}

		// TODO ゴールのフレーム -------------
		const int gfx = -81, gfy = 250, goals = 192;
		for (int i = 0; i <= 6; i++)
		{
			//Rect nestFrame[5] = { {nestPos.asPoint(), 80, 50} };
			Rect nestFrame[5] = { { gfx + goals * i, gfy, 100, 50 } };
			for (auto& i : nestFrame)
				i.draw(ColorF(Palette::Green, 1));
			if (isHitFrame = nestFrame->intersects(frogCol))
			{
				isDead = true;
				if (isDead)
					respawnTimer.start();
			}
		}

#pragma endregion

		String prefixes[2] = { U"スコア: ", U"タイム: " };
		fontBold(prefixes[0], score.current).drawAt(double(sceneWidth / 2), fontBold.fontSize());
		fontBold(prefixes[1], playTime).drawAt(double(sceneWidth / 2), fontBold.fontSize() * 2);
		fontBold(count).draw(Scene::Center() + Vec2(0, fontBold.fontSize()));
		for (auto& i : frogNests)
			i.Draw();

#if _DEBUG

		if (KeyE.pressed())
			frogPos.y = 640;

		font30(Cursor::Pos()).draw(0, 0, Palette::White);
		font30(frogPos).draw(0, font30.fontSize());
		font30(frogVel).draw(0, font30.fontSize() * 2);
		font30(U"LStick: {}"_fmt(Controller(0).LStick())).draw(0, font30.fontSize() * 3);
#endif
	}
}
