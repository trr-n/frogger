#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

//using V2 = std::map<std::string, Vec2>;
using str = std::string;

class Turtle {
	Rect drawregion{ 0, 0, 0, 0 };

public:
	Texture texture;
	Stopwatch sw;
	const int width = 64, height = 64;
	Vec2 position, velocity;

	Turtle()
	{
		Initialize(Texture(), Vec2::Zero());
	}

	/// @brief 亀の初期値を指定
	/// @param texture テクスチャ
	/// @param position 座標
	/// @param speed 速度
	Turtle(Texture texture, Vec2 position, int speed)
	{
		Initialize(texture, position);
		velocity.set(speed, 0);
	}

	/// @brief 初期化、タイマースタート
	/// @param texture テクスチャ
	/// @param position 座標
	void Initialize(Texture _texture, Vec2 _position)
	{
		_texture = _texture;
		_position = _position;
		sw.start();
	}

	/// @brief 移動、アニメーション更新
	void Update()
	{
		position += velocity * Scene::DeltaTime();
		drawregion.set(GetIndex() * width, 0, width, height);
	}

	/// @brief 描画
	void Draw()
	{
		texture(drawregion).draw(position);
	}

	/// @brief 描画中の
	/// @param totalIndex 合計アニメーションパターン数
	/// @param animeSpan アニメーションスピード
	/// @return 描画中
	int GetIndex(int totalIndex = 8, int animeSpan = 500)
	{
		return (sw.ms() / animeSpan) % totalIndex;
	}

	/// @brief 沈んでいるかの判定
	/// @param plunkingAnim 沈み始めるアニメーションパターン 
	/// @return 沈んでいたら true, 浮いていたら false
	bool IsPlunking(int plunkingAnim = 6)
	{
		return plunkingAnim < GetIndex();
	}
};

class FrogNest
{
public:
	Rect nest; // 巣の位置と大きさと同じ判定を持つ矩形
	Texture frogSits; // 座っている画像 
	bool isFrogSits; // カエルが座っているか

	// カエルが座っていたら描画（IsFrogSits==true）、そうでなかったら、何も描かない
	void Draw()
	{

	}

	// プレイヤーの判定とnestをチェックしてカエルが座っていなかったら IsFrogSits=trueにして return false で抜ける
	// そうでなかったら、return でtrue を返す（プレイヤーミス）
	bool HitCheck(Rect playerRect)
	{

		return true; // ここがtrueならプレイヤーミス
	}
};

void MyGame::Play()
{
	if (SimpleGUI::ButtonAt(U"Title", Vec2(0, Scene::Height())))
	{
		ChangeScene(&MyGame::Title);
	}

	Scene::Resize(SceneX, SceneY);
	Scene::SetBackground(Color(0, 0, 26));

	TextureRegion frogAnims[8] =
	{
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
	/// @brief かえるの座標
	Vec2 frogPos(FrogSpawn);
	/// @brief かえるの速度
	Vec2 frogVel(0, 0);
	/// @brief ゴールの座標 
	Vec2 goalPos(15, 220);
	/// @brief 画面外待機場所
	Vec2 frogOut(2048, 2048);

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
	/// @brief 0-4: cars
	/// @brief 5: log
	/// @brief 6: turtle
	Speeds speeds(1.33, 70 / 60, 1.33, 180 / 60, 65 / 60, 1.33, 60 / 60);
	//Speeds speeds(80, 70, 80, 180, 65, 80, 60);

	Array<GameObject> cars, logs;
	Array<Turtle> turtles;

	int carLanes[] = { 0, 0, 2, 2, 4 };
	for (int i = 0; i < 3; i++)
	{
		/// @brief ランダムで車間距離を生成
		auto spaceAdd = [&]() { return i * Random(200, 400); };
		cars << GameObject(car0, spawnPos.rightBottom - Vec2(spaceAdd(), Tile * 0), -speeds.car0);
		cars << GameObject(car1, spawnPos.leftBottom + Vec2(spaceAdd(), Tile * 0), speeds.car1);
		cars << GameObject(car2, spawnPos.rightBottom - Vec2(spaceAdd(), Tile * 2), -speeds.car2);
		cars << GameObject(car3, spawnPos.leftBottom + Vec2(spaceAdd(), -Tile * 2), speeds.car3);
		cars << GameObject(car4, spawnPos.rightBottom - Vec2(spaceAdd(), Tile * 4), -speeds.car4);
	}

	int logLanes[3] = { 0,2,3 };
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

	while (Update())
	{
#if _DEBUG
		if (KeySpace.down())
		{
			sw.start();
		}
#endif

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

		/// @brief ジャンプと効果音再生
		auto Jumping = [&](int _pattern, int _x, int _y)
		{
			frogPattern = _pattern + 1;
			frogVel.set(_x, _y);
			jumpSound.playOneShot();
			count = 0;
		};

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
		else {
			auto velocity = frogVel;
			switch (count)
			{
			case 0:
				velocity *= 8;
				break;
			case 1:
				velocity *= 8;
				break;
			case 2:
				velocity *= 8;
				break;
			case 3:
				velocity *= 8;
				break;
			case 4:
				velocity *= 8;
				break;
			case 5:
				velocity *= 8;
				break;
			case 6:
				velocity *= 8;
				break;
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

		/*/// @brief リスポーン,効果音再生
		auto PlayF = [&](Audio a)
		{
			a.playOneShot();
			frogPos = FrogSpawn;
		};*/

		// 画面外にでないように移動可能な範囲を限定
		frogPos.x = std::clamp((int)frogPos.x, 0, sceneWidth);
		frogPos.y = std::clamp((int)frogPos.y, 0, sceneHeight);

		Rect frogCol(frogPos.asPoint() + Point(16, 16), (frogsForward.width() / 2) - 32, frogsForward.height() - 32);
		frogCol.draw(ColorF(Palette::Cyan, 0.25));

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
			frogPos = frogOut;
			plunkTimer.start();
			plunkSound.playOneShot();
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
				log0.RideOn(&frogPos);
				*(log0.playerPosPtr) += Vec2(1, 0);
			}
			log0.Update();

			//log0.Returns(1200, lowerLeft.x);

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

			//  
			if (turtle0.position.x < -300)
			{
				turtle0.position.x = lowerRight.x;
			}
		}

		// ピンクの足場(中間地点) -------------
		for (auto pinkie = 0; pinkie < Scene::Width(); pinkie += Tile)
		{
			Vec2 f0(pinkie, sceneHeight);
			Vec2 f1(pinkie, sceneHeight / 2);
			halfway.draw(pinkie, sceneHeight);
			halfway.draw(pinkie, 640);

			Rect fCol0(f0.asPoint(), halfway.width(), halfway.height());
			Rect fCol1(f1.asPoint(), halfway.width(), halfway.height());
			if (fCol0.intersects(frogPos) || fCol1.intersects(frogPos))
			{
			}
		}

		// ゴールのフレーム -------------
		Rect goalFrame(goalPos.asPoint(), 80, 50);
		if (goalFrame.intersects(frogCol))
		{
			// 音変更予定
			squash.playOneShot();
			frogPos = FrogSpawn;
		}

		goalFrame.draw(ColorF{ Palette::White, .25 });

		// ゴール -------------
		for (auto goal0 = 0; goal0 < 5; goal0++)
		{
			const int gx = 118, gy = 253;

			Rect goalsCol[5] = {
				{ gx + 160 * goal0, gy, 60, 60 }
			};

			for (auto& j : goalsCol)
			{
				j.draw(Palette::Red);
			}

			if (goalsCol->intersects(frogCol))
			{
			}
		}

		// ---------------描画-------------------

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

		goal.draw(goalPos);

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

			// 溺死 未実装 ----------------------------------

			// しんだら2秒間休憩
			if (respawnTimer.sF() >= breakTime)
			{
				frogPos = FrogSpawn;
				isDead = false;
				moving = 1;
				respawnTimer.reset();
			}
		}

#if _DEBUG

		if (KeyE.pressed()) frogPos.y = 576;

		font30(Cursor::Pos()).draw(0, 0, Palette::White);
		font30(frogPos).draw(0, font30.fontSize());
		font30(frogVel).draw(0, font30.fontSize() * 2);
		font30().draw(0, font30.fontSize() * 3);

#endif
	}
}
