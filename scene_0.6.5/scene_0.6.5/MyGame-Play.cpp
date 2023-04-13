# include <stdio.h>
# include <iostream>
# include <string>
# include <map>
# include "stdafx.h"
// # include "PlayOneShot.h"
// # include <NamedParameter.hpp>

//using V2 = std::map<std::string, Vec2>;
using str = std::string;

/// @brief 速度の値を管理
class Speeds {
public:
	int car0, car1, car2, car3, car4;
	int log, turtle;
	Speeds(void);

	/// @brief 移動速度を指定
	/// @param c0 車
	/// @param c1 ↓
	/// @param c2 ↓ 
	/// @param c3 ↓
	/// @param c4 __
	/// @param log 丸太
	/// @param turtle 亀
	Speeds(int c0, int c1, int c2, int c3, int c4, int log, int turtle) {
		car0 = c0;
		car1 = c1;
		car2 = c2;
		car3 = c3;
		car4 = c4;
		Speeds::log = log;
		Speeds::turtle = turtle;
	}
};

/// @brief 生成座標を管理
class SpawnPosition
{
public:
	Point leftUp, leftBottom, rightUp, rightBottom;

	SpawnPosition() {
		leftUp = Point(0, 0);
		leftBottom = Point(0, 0);
		rightUp = Point(0, 0);
		rightBottom = Point(0, 0);
	}

	/// @brief 上下レーンの障害物の生成座標を指定
	/// @param _leftUp 上段の左側
	/// @param _leftBottom 下段の左側
	/// @param _rightUp 上段の右側
	/// @param _rightBottom 下段の右側
	SpawnPosition(Point _leftUp, Point _leftBottom, Point _rightUp, Point _rightBottom)
	{
		leftUp.set(_leftUp);
		leftBottom.set(_leftBottom);
		rightUp.set(_rightUp);
		rightBottom.set(_rightBottom);
	}
};

class GameObject {
public:
	Vec2 position;
	Vec2 velocity;
	Texture texture;

	/// @brief 初期値を指定
	/// @param $tex テクスチャ
	/// @param $pos 座標
	/// @param $speed 移動速度
	GameObject(Texture $tex, Vec2 $pos, int $speed) {
		texture = $tex;
		position = $pos;
		velocity.set($speed, 0);
	}

	/// @brief 移動、更新
	void Update() {
		position += velocity * Scene::DeltaTime();
	}

	/// @brief 描画
	void Draw() {
		texture.draw(position);
	}
};

class Turtle {
	Rect drawregion{ 0, 0, 0, 0 };

public:
	Texture texture;
	Stopwatch sw;
	const int width = 64, height = 64;
	Vec2 position, velocity;

	Turtle() {
		Initialize(Texture(), Vec2::Zero());
	}

	/// @brief 亀の初期値を指定
	/// @param texture テクスチャ
	/// @param position 座標
	/// @param speed 速度
	Turtle(Texture texture, Vec2 position, int speed) {
		Initialize(texture, position);
		velocity.set(speed, 0);
	}

	/// @brief 初期化、タイマースタート
	/// @param texture テクスチャ
	/// @param position 座標
	void Initialize(Texture texture, Vec2 position) {
		texture = texture;
		position = position;
		sw.start();
	}

	/// @brief 移動、アニメーション更新
	void Update() {
		position += velocity * Scene::DeltaTime();
		drawregion.set(GetIndex() * width, 0, width, height);
	}

	/// @brief 描画
	void Draw() {
		texture(drawregion).draw(position);
	}

	/// @brief 描画中の
	/// @param totalIndex 合計アニメーションパターン数
	/// @param animeSpan アニメーションスピード
	/// @return 描画中
	int GetIndex(int totalIndex = 8, int animeSpan = 500) {
		return (sw.ms() / animeSpan) % totalIndex;
	}

	/// @brief 沈んでいるかの判定
	/// @param plunkingAnim 沈み始めるアニメーションパターン 
	/// @return 沈んでいたら true, 浮いていたら false
	bool IsPlunking(int plunkingAnim = 6) {
		return plunkingAnim < GetIndex();
	}
};

//void OutOfScreen(
//	GameObject object,
//	s3d::Vec2 position,
//	double xy,
//	Vec2 turn
//) {
//	if (object.position.xy) {
//		object.position.xy = turn.xy;
//	}
//};

void MyGame::Play() {
	if (SimpleGUI::ButtonAt(U"Title", Vec2(0, Scene::Height()))) {
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
	/// @brief かえるの座標
	Vec2 frogPos(FrogSpawn);
	/// @brief かえるの速度
	Vec2 frogVel(0, 0);
	/// @brief ゴールの座標 
	Vec2 goalPos(15, 220);

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
	Speeds speeds(80, 70, 80, 180, 65, 80, 60);

	Array<GameObject> cars, logs;
	Array<Turtle> turtles;

	int carLanes[] = { 0, 0, 2, 2, 4 };
	for (int i = 0; i < 3; i++) {
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
	for (auto i = 0; i < 3; i++) {
		logs << GameObject(
			log,
			upperLeft - Vec2(i * Random(logMin, logMax), Tile * logLanes[i]),
			speeds.log
		);
	}

	// 3匹セットで生成
	int turtleLanes[] = { 0, 2 };
	for (auto i = 0; i < 5; i++) {
		for (auto j = 0; j < 2; j++) {
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

	const int Up = 0, Back = 2, left = 4, right = 6;
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

	while (Update()) {
		if (KeySpace.down()) {
			sw.start();
		}

		/// @brief 移動キー
		/// @brief 0 forward
		/// @brief 1 back
		/// @brief 2 left
		/// @brief 3 right
		bool keysDown[4] = {
			(KeyUp | KeyW).down(),
			(KeyDown | KeyS).down(),
			(KeyLeft | KeyA).down(),
			(KeyRight | KeyD).down()
		};

		// ---------------移動-------------------

		/// @brief ジャンプと効果音再生
		auto Jumping = [&](int _pattern, int _x, int _y) {
			frogPattern = _pattern + 1;
			frogVel.set(_x, _y);
			jumpSound.playOneShot();
			count = 0;
		};

		// 地に足がついていて且つキーを押していたら移動
		if (frogPattern % 2 == 0) {
			if (keysDown[0]) {
				Jumping(Up, 0, -moving);
			}

			else if (keysDown[1]) {
				Jumping(Back, 0, moving);
			}

			else if (keysDown[2]) {
				Jumping(left, -moving, 0);
			}

			else if (keysDown[3]) {
				Jumping(right, moving, 0);
			}
		}

		// 移動時のアニメーション調整用
		else {
			auto velocity = frogVel;
			switch (count) {
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

			if (count > 7) {
				frogPattern -= 1;
			}
		}

		// 画面外にでないように移動可能な範囲を限定
		frogPos.x = std::clamp((int)frogPos.x, 0, sceneWidth);
		frogPos.y = std::clamp((int)frogPos.y, 0, sceneHeight);

		Rect frogCol(frogPos.asPoint() + Point(16, 16), (frogsForward.width() / 2) - 32, frogsForward.height() - 32);
		frogCol.draw(ColorF(Palette::Cyan, 0.25));

		// くるまの移動と当たり判定
		for (auto& i : cars) {
			i.Update();

			// くるま当たり判定
			Rect obsCol(i.position.asPoint(), i.texture.width(), i.texture.height());
			if (obsCol.intersects(frogCol) && !isDead) {
				squash.playOneShot();
				isDead = true;
				if (isDead) {
					respawnTimer.start();
				}
			}

			// 画面外にでたら戻る
			if (i.position.x < -300) i.position.x = lowerRight.x;
			if (i.position.x > 1200) i.position.x = lowerLeft.x;
		}

		Rect plunkArea(0, 325, 900, 300);
		plunkArea.draw(ColorF(Palette::Skyblue, .25));

# if _DEBUG
		static double inc = 1;
		if (KeyJ.pressed()) inc += .001;
		if (KeyK.pressed()) inc -= .001;

		if (KeyE.pressed()) frogPos.y = 576;
#endif

		// 丸太 -------------
		for (auto& log : logs) {
			log.Update();

			Rect logCol(log.position.asPoint(), log.texture.width(), log.texture.height());
			if (logCol.intersects(frogCol)) {
				frogPos.x += speeds.log * (inc / 60.0);
			}

			// ループ
			if (log.position.x > 1200) {
				log.position.x = lowerLeft.x;
			}
		}

		// 亀 -------------
		for (auto& turtle : turtles) {
			turtle.Update();

			Rect turtleCol(turtle.position.asPoint(), Tile, Tile);
			// 左(亀の進行方向)に押し出される
			if (turtleCol.intersects(frogCol)) {
				frogPos.x -= speeds.turtle * Scene::DeltaTime();

				if (turtle.IsPlunking()) {
					frogPos = FrogSpawn;
					plunkSound.playOneShot();
				}
			}

			if (turtle.position.x < -300) {
				turtle.position.x = lowerRight.x;
			}
		}

		// ピンクの足場(中間地点) -------------
		for (auto pinkie = 0; pinkie < Scene::Width(); pinkie += Tile) {
			Vec2 f0(pinkie, sceneHeight);
			Vec2 f1(pinkie, sceneHeight / 2);
			footing.draw(pinkie, sceneHeight);
			footing.draw(pinkie, 640);

			// 当たり判定
			Rect fCol0(f0.asPoint(), footing.width(), footing.height());
			Rect fCol1(f1.asPoint(), footing.width(), footing.height());
			if (fCol0.intersects(frogPos) || fCol1.intersects(frogPos)) {
			}
		}

		// ゴールのフレーム -------------
		Rect goalFrame(goalPos.asPoint(), 80, 50);
		if (goalFrame.intersects(frogCol)) {
		}

		goalFrame.draw(ColorF{ Palette::White, .25 });

		// ゴール -------------
		for (auto goal = 0; goal < 5; goal++) {
			const int gx = 118, gy = 253;

			Rect goalsCol[5] = {
				{ gx + 160 * goal, gy, 60, 60 }
			};

			for (auto& j : goalsCol) {
				j.draw(Palette::Red);
			}

			if (goalsCol->intersects(frogCol)) {
			}
		}

		// ---------------描画-------------------

		for (auto& cars : cars) {
			cars.Draw();
		}

		for (auto& logs : logs) {
			logs.Draw();
		}

		for (auto& turtles : turtles) {
			turtles.Draw();
		}

		goal.draw(goalPos);

		if (sw.isRunning() && sw.sF() < 3) {
			Rect(frogPos.asPoint(), Tile, Tile).draw(Palette::Red);
		}

		else {
			frogAnims[frogPattern].draw(frogPos);
		}

		if (isDead) {
			// 一定時間おきにパターン変化
			moving = 0;

			// 車とぶつかったとき
			int pattern = (int)respawnTimer.sF() / (breakTime / 7.0);
			auto w = deadPattern.width() / 7;
			deadPattern(pattern * w, 0, w, Tile).draw(frogPos);

			// 溺死 未実装 ----------------------------------

			// しんだら2秒間
			if (respawnTimer.sF() >= breakTime) {
				frogPos = FrogSpawn;
				isDead = false;
				moving = 1;
				respawnTimer.reset();
			}
		}

#if _DEBUG

		font30(Cursor::Pos()).draw(0, 0, Palette::White);
		font30(frogPos).draw(0, font30.fontSize());
		font30(count).draw(0, font30.fontSize() * 2);
		font30(123).draw(0, font30.fontSize() * 3);

#endif
	}
}

/*
TODO
ゴールしたらでかいかえるを表示
そのかえるを数えてクリアか判定
*/
