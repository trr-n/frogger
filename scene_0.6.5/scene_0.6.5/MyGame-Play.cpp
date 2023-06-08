#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

//using V2 = std::map<std::string, Vec2>;
using str = std::string;

void MyGame::Play()
{
	Print();

	if (SimpleGUI::ButtonAt(U"Title", Vec2(0, Scene::Height())))
	{
		ChangeScene(&MyGame::Title);
	}

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

	/// @brief かえるの生成座標、初期地点
	const Vec2 FrogSpawn(448, 1024);

	/// @brief 画面外待機場所
	const Vec2 frogOut(2048, 2048);

	/// @brief かえるの座標
	Vec2 frogPos(FrogSpawn);

	/// @brief かえるの速度
	Vec2 frogVel(0, 0);

	/// @brief ゴールの座標 
	Vec2 nestPos(0, 220);

	Vec2 upperRight(960, 576);
	Vec2 upperLeft(-128, 512);
	Vec2 lowerRight(900, 960);
	Vec2 lowerLeft(-128, 896);

	/// @brief 上下段オブジェ生成座標
	/// @brief 1: left up
	/// @brief 2: left bottom    
	/// @brief 3: right up
	/// @brief 4: right bottom
	SpawnPosition spawnPos(
		Point(-128, 512),
		Point(-128, 896),
		Point(960, 576),
		Point(900, 960)
	);

	/// @brief シーンの横幅
	int sceneWidth = Scene::Width() - Tile;
	/// @brief シーンの縦幅
	int sceneHeight = Scene::Height() - Tile * 2;

	/// @brief オブジェクトの移動速度
	Speeds speeds(
		80 / 60,	// 0: cars
		70 / 60,	// 1: cars
		80 / 60,	// 2: cars
		180 / 60,	// 3: cars
		65 / 60,	// 4: cars
		80 / 60,	// 5: log
		60 / 60		// 6: turtle
	);

	Array<GameObject> cars, logs;
	Array<Turtle> turtles;

	int carLanes[] = { 0, 0, 2, 2, 4 };
	for (int i = 0; i < 3; i++)
	{
		/// @brief ランダムで車間距離を設定
		auto spaceAdd = [&]() { return i * Random(200, 400); };
		cars << GameObject(car0, spawnPos.rightBottom - Vec2(spaceAdd(), Tile * 0), -speeds.car0);
		cars << GameObject(car1, spawnPos.leftBottom + Vec2(spaceAdd(), Tile * 0), speeds.car1);
		cars << GameObject(car2, spawnPos.rightBottom - Vec2(spaceAdd(), Tile * 2), -speeds.car2);
		cars << GameObject(car3, spawnPos.leftBottom + Vec2(spaceAdd(), -Tile * 2), speeds.car3);
		cars << GameObject(car4, spawnPos.rightBottom - Vec2(spaceAdd(), Tile * 4), -speeds.car4);
	}

	int logLanes[3] = { 0, 2, 3 };
	int logMin = 400, logMax = 700;

	for (auto i = 0; i < 3; i++)
	{
		logs << GameObject(
			log,
			upperLeft - Vec2(i * Random(logMin, logMax), Tile * logLanes[i]),
			speeds.log
		);
	}

	// 3匹セットで生成
	int turtleLanes[] = { 0, 2 };
	for (auto i = 0; i < 5; i++)
	{
		for (auto j = 0; j < 2; j++)
		{
			turtles << Turtle(
				turtleAnims,
				upperRight - Vec2(Tile * j, Tile * turtleLanes[j]),
				-speeds.turtle
			);
		}
	}

	Stopwatch sw;

	/// @brief 再生成するまでの時間を計測するタイマー
	Stopwatch respawnTimer;

	const int Up = 0, Back = 2, Left = 4, Right = 6;
	/// @brief アニメーションパターン
	int frogPattern = Up;

	/// @brief 移動速度
	int moving = 1;

	/// @brief アニメーションパターンのカウント
	int count = 0;

	/// @brief 死んでから再度動けるようになるまでの時間
	int breakTime = 2;

	/// @brief 死亡判定フラグ
	bool isDead = false;

	/// @brief キーボード操作のフラグ
	bool isCtrl = true;

	Vec2 sliderPosition(0, 0);

	// BGM
	float bgmVolume = 0;
	bgm1.play();
	bgm1.setVolume(bgmVolume);

	//FrogNest frogTest(frogSitting);
	Stopwatch nesting;

	const int Y = 246;
	FrogNest frogNests[5] = {
		FrogNest(33, Y, Tile, Tile, frogSitting),
		FrogNest(223, Y, Tile, Tile, frogSitting),
		FrogNest(414, Y, Tile, Tile, frogSitting),
		FrogNest(606, Y, Tile, Tile, frogSitting),
		FrogNest(803, Y, Tile, Tile, frogSitting)
	};
	/*FrogNest frogNests[5];
	for (auto i = 0; i > 5; i++) {
		frogNests[i] = FrogNest(33 + i * 120, Y, Tile, Tile, frogSitting);
	}*/

	Score score = { 1, 100, 334, -10 };

	while (Update())
	{
#if _DEBUG
		if (KeySpace.down())
		{
			sw.start();
		}
#endif
		// BGMの音量調節スライダー
		//SimpleGUI::Slider(U"Volume", bgmVolume, sliderPosition);

		/// @brief 移動キー
		/// @brief 0 forward
		/// @brief 1 back
		/// @brief 2 left
		/// @brief 3 right
		bool keysDown[4] = {
			(KeyUp | KeyW).down() && isCtrl,
			(KeyDown | KeyS).down() && isCtrl,
			(KeyLeft | KeyA).down() && isCtrl,
			(KeyRight | KeyD).down() && isCtrl
		};

		// ---------------移動-------------------

		/// @brief ジャンプの処理
		auto Jumping = [&](int _pattern, int _x, int _y)
		{
			frogPattern = _pattern + 1;

			frogVel.set(_x, _y);

			jumpSound.playOneShot();

			count = 0;

			score.Add(score.jump);
		};

		if (squash.posSec() >= squash.lengthSec() - 0.1)
		{
			FrogNest::intoNest = false;

			frogVel = Vec2(0, 0);
		}

		while (!FrogNest::intoNest)
		{
			// キー入力
			if (frogPattern % 2 == 0)
			{
				if (keysDown[Up])
				{
					Jumping(Up, 0, -moving);
				}

				else if (keysDown[Back / 2])
				{
					Jumping(Back, 0, moving);
				}

				else if (keysDown[Left / 2])
				{
					Jumping(Left, -moving, 0);
				}

				else if (keysDown[Right / 2])
				{
					Jumping(Right, moving, 0);
				}
			}

			// 移動時のアニメーション調整用
			else
			{
				auto velocity = frogVel;

				switch (count)
				{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
					velocity *= 8;
					break;

				default:
					_ASSERT_EXPR(false, L"error");
					break;
				}

				frogPos += velocity;

				count++;

				if (count > 7)
				{
					frogPattern -= 1;
				}
			}

			break;
		}

		// 画面外にでないように移動可能な範囲を限定
		frogPos.x = std::clamp((int)frogPos.x, 0, sceneWidth);
		frogPos.y = std::clamp((int)frogPos.y, 220, sceneHeight);

		Rect frogCol(frogPos.asPoint() + Point(16, 16), (frogsForward.width() / 2) - 32, frogsForward.height() - 32);
		frogCol.draw(ColorF(Palette::Cyan, 0.25));

#pragma region 当たり判定
		// くるまの移動と当たり判定
		for (auto& i : cars)
		{
			i.Update();

			// くるま当たり判定
			Rect obsCol(i.position.asPoint(), i.texture.width(), i.texture.height());
			if (obsCol.intersects(frogCol) && !isDead)
			{
				squash.playOneShot();

				isDead = true;

				if (isDead)
				{
					respawnTimer.start();
				}
			}

			// 画面外にでたら戻る
			if (i.position.x < -300) i.position.x = lowerRight.x;
			if (i.position.x > 1200) i.position.x = lowerLeft.x;
		}

		// 水没処理
		Rect plunkArea(0, 325, 900, 300);
		Stopwatch plunkTimer;
		plunkArea.draw(ColorF(Palette::Skyblue, .25));
		if (frogCol.intersects(plunkArea))
		{
			//frogPos = frogOut;
			plunkTimer.start();
			//plunkSound.playOneShot();
			score.Add(score.hit);
		}

		if (plunkTimer.sF() >= 2)
		{
			frogPos = FrogSpawn;
			plunkTimer.reset();
		}

		// 丸太 -------------
		for (auto& log0 : logs)
		{
			Rect logCol(log0.position.asPoint(), log0.texture.width(), log0.texture.height());
			if (logCol.intersects(frogCol))
			{
				log0.playerPosPtr = &frogPos;
				log0.Riding(&frogPos);
				*(log0.playerPosPtr) += Vec2(1, 0);
			}
			log0.Update();

			if (log0.position.x > 1200)
			{
				log0.position.x = lowerLeft.x;
			}
		}

		// 亀 -------------
		for (auto& turtle0 : turtles)
		{
			turtle0.Update();

			Rect turtleCol(turtle0.position.asPoint(), Tile, Tile);
			if (turtleCol.intersects(frogCol))
			{
				frogPos.x -= speeds.turtle * Scene::DeltaTime();
			}

			if (turtle0.position.x < -300)
			{
				turtle0.position.x = lowerRight.x;
			}
		}

		// ピンクの足場(中間地点) -------------
		for (auto pinkie = 0; pinkie < Scene::Width(); pinkie += Tile)
		{
			Vec2 f0(pinkie, sceneHeight), f1(pinkie, sceneHeight / 2);
			halfway.draw(pinkie, sceneHeight);
			halfway.draw(pinkie, 640);

			Rect fCol0(f0.asPoint(), halfway.width(), halfway.height());
			Rect fCol1(f1.asPoint(), halfway.width(), halfway.height());
			if (fCol0.intersects(frogPos) || fCol1.intersects(frogPos))
			{
			}
		}

		for (auto& nest : frogNests)
		{
			nest.HitCheck(frogCol);
		}
		// 巣に入った瞬間ならす
		if (FrogNest::intoNest && !squash.isPlaying())
		{
			//score += NestPoint;
			score.Add(score.nest);
			squash.play();
		}
		if (score.current >= 500)
		{
			ChangeScene(&MyGame::Title);
		}

		// ゴールのフレーム -------------
		Rect nestFrame(nestPos.asPoint(), 80, 50);
		if (nestFrame.intersects(frogCol))
		{
			// 音変更予定
			//squash.playOneShot();
			frogPos = FrogSpawn;
		}

#pragma endregion

#pragma region 描画
		nestFrame.draw(ColorF{ Palette::White, .25 });

		// ゴール -------------

		// 一番左の巣の当たり判定の座標、間隔
		const int gx = 33, gy = 250, neste = 193;

		for (int nest0 = 0; nest0 < 5; nest0++)
		{
			Rect nestsCol[5] = {
				{ gx + neste * nest0, gy, 60, 30 }
			};

			for (auto& j : nestsCol)
			{
				j.draw(ColorF(Palette::Red, .5));
			}

			// ゴールしたら
			if (FrogNest::intoNest)
			{
				// かえるを初期座標にもどす
				frogPos = FrogSpawn;
			}
		}

		for (auto& c : cars)
		{
			c.Draw();
		}

		for (auto& l : logs)
		{
			l.Draw();
		}

		for (auto& t : turtles)
		{
			t.Draw();
		}

		nest.draw(nestPos);

		if (sw.isRunning() && sw.sF() < 3)
		{
			Rect(frogPos.asPoint(), Tile, Tile).draw(Palette::Red);
		}

		else
		{
			frogAnims[frogPattern].draw(frogPos);
		}

		if (isDead)
		{
			// 一定時間おきにパターン変化
			moving = 0;
			// 車とぶつかったとき
			int pattern = (int)(respawnTimer.sF() / (breakTime / 7.0));
			auto w = deadPattern.width() / 7;

			deadPattern(pattern * w, 0, w, Tile).draw(frogPos);

			// 溺死 未実装

			// しんだら2秒間休憩
			if (respawnTimer.sF() >= breakTime)
			{
				frogPos = FrogSpawn;
				isDead = false;
				moving = 1;
				respawnTimer.reset();
			}
		}
#pragma endregion

		fontBold(score.current).drawAt(double(sceneWidth / 2), fontBold.fontSize());

#if _DEBUG

		if (KeyE.pressed())
		{
			frogPos.y = 576;
		}

		for (auto& i : frogNests)
		{
			i.Draw();
		}

		font30(Cursor::Pos()).draw(0, 0, Palette::White);

		font30(frogPos).draw(0, font30.fontSize());

		font30(frogVel).draw(0, font30.fontSize() * 2);
#endif
	}
}
