# include "stdafx.h"
// # include "PlayOneShot.h"

class GameObject {
public:
	Vec2 position;
	Vec2 velocity;
	Texture texture;

	GameObject(Texture $tex, Vec2 $pos, int $speed) {
		texture = $tex;
		position = $pos;
		velocity.set($speed, 0);
	}

	void Update() {
		position += velocity * Scene::DeltaTime();
	}

	void Draw() {
		texture.draw(position);
	}
};

class Turtle {
	Rect drawRegion{ 0, 0, 0, 0 };
public:
	Texture texture;
	Stopwatch sw;
	const int width = 64, height = 64;
	Vec2 position;
	Vec2 velocity;

	Turtle() {
		Initialize(Texture(), Vec2::Zero());
	}

	Turtle(Texture $texture, Vec2 $position, int $speed) {
		Initialize($texture, $position);
		velocity.set($speed, 0);
	}

	void Initialize(Texture _texture, Vec2 _position) {
		texture = _texture;
		position = _position;
		sw.start();
	}

	void TurtleUpdate() {
		position += velocity * Scene::DeltaTime();
		drawRegion.set(GetTurtleIndex() * width, 0, width, height);
	}

	void TurtleDraw() {
		texture(drawRegion).draw(position);
	}

	int GetTurtleIndex(int _i = 8) {
		int animationSpan = 500; // 1/2秒で切り替え
		int animationIndex = (sw.ms() / animationSpan) % _i;

		return animationIndex;
	}

	bool IsPlunking() {
		return 6 < GetTurtleIndex();
	}
};

void MyGame::Play() {
	if (SimpleGUI::ButtonAt(U"Title", Vec2{ 0, Scene::Height() })) {
		ChangeScene(&MyGame::Title);
	}

	Scene::Resize(SceneX, SceneY);
	Scene::SetBackground(Color(0, 0, 26));

	TextureRegion frogAnims[8] = {
		frogsForward(0,		0, Tile * 1, Tile),
		frogsForward(Tile,	0, Tile * 2, Tile * 2),
		frogsBack(0,		0, Tile * 1, Tile),
		frogsBack(Tile,	0, Tile * 2, Tile * 2),
		frogsLeft(0,		0, Tile * 1, Tile),
		frogsLeft(Tile,	0, Tile * 2, Tile * 2),
		frogsRight(Tile,	0, Tile * 2, Tile * 2),
		frogsRight(0,		0, Tile * 1, Tile)
	};

	Vec2 frogSpawn(448, 1216);
	Vec2 frogPos(frogSpawn);
	Vec2 frogVel(0, 0);
	Vec2 goalPos(15, 220);

	Vec2 upperRight(960, 576);
	Vec2 upperLeft(-128, 512);
	Vec2 lowerRight(900, 960);
	Vec2 lowerLeft(-128, 896);

	const int sceneW = Scene::Width() - Tile;
	const int sceneH = Scene::Height() - Tile * 2;
	const int ladderSpeed = 70;
	const int car1Speed = 80;
	const int car2Speed = 70;
	const int car3Speed = 80;
	const int car4Speed = 180;
	const int car5Speed = 65;

	Array<GameObject> obs;
	Array<GameObject> logs;
	Array<Turtle> turtles;

	// 車間距離 できればランダム
	auto carSpace = Random(256, 512);
	for (auto i = 0; i < 3; i++) {
		obs << GameObject(car1, lowerRight - Vec2(i * carSpace, Tile * 0), -car1Speed);
		obs << GameObject(car2, lowerLeft + Vec2(i * carSpace, Tile * 0), car2Speed);
		obs << GameObject(car3, lowerRight - Vec2(i * carSpace, Tile * 2), -car3Speed);
		obs << GameObject(car4, lowerLeft + Vec2(i * carSpace, -Tile * 2), car4Speed);
		obs << GameObject(car5, lowerRight - Vec2(i * carSpace, Tile * 4), -car5Speed);
	}

	for (auto i = 0; i < 5; i++) {
		logs << GameObject(log, upperLeft - Vec2(i * carSpace, Tile * 0), ladderSpeed);
		logs << GameObject(log, upperLeft - Vec2(i * carSpace, Tile * 2), ladderSpeed);
		logs << GameObject(log, upperLeft - Vec2(i * carSpace, Tile * 3), ladderSpeed);
	}

	// 3匹セットで生成
	for (auto i = 0; i < 5; i++) {
		for (auto j = 0; j < 3; j++) {
			turtles << Turtle(turtleAnims, upperRight + Vec2(Tile * j, 0), -ladderSpeed);
			turtles << Turtle(turtleAnims, upperRight - Vec2(Tile * j, Tile * 2), -ladderSpeed);
		}
	}

	Stopwatch sw;

	const int Up = 0, Back = 2, Left = 4, Right = 6;
	int frogPattern = Up;
	int frogDirection = -1;

	int move = 1;
	int count = 0;

	while (Update()) {
		if (KeySpace.down()) {
			sw.start();
		}

		bool forwardDown = (KeyUp | KeyW).down();
		bool leftDown = (KeyLeft | KeyA).down();
		bool backDown = (KeyDown | KeyS).down();
		bool rightDown = (KeyRight | KeyD).down();

		// ---------------移動-------------------

		auto MoveJump = [&](int _pattern, int _x, int _y) {
			frogPattern = _pattern + 1;
			frogVel.set(_x, _y);
			jumpSound.playOneShot();

			count = 0;
		};

		// 地上か
		if (frogPattern % 2 == 0)
		{
			if (forwardDown)
				MoveJump(Up, 0, -move);

			else if (backDown)
				MoveJump(Back, 0, move);

			else if (leftDown)
				MoveJump(Left, -move, 0);

			else if (rightDown)
				MoveJump(Right, move, 0);
		}
		// 移動中
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
				break;
			}

			frogPos += velocity;
			count++;

			if (count > 7)
				frogPattern -= 1;
		}

		font30(Cursor::Pos()).draw(0, 0, Palette::White);
		font30(count).draw(0, font30.fontSize() * 2);

		// かえるが画面外にでないように
		frogPos.x >= sceneW ? frogPos.x = sceneW : frogPos.x;
		frogPos.x <= 0 ? frogPos.x = 0 : frogPos.x;
		frogPos.y >= sceneH ? frogPos.y = sceneH : frogPos.y;
		frogPos.y <= 0 ? frogPos.y = 0 : frogPos.y;

		Rect frogCol(frogPos.asPoint() + Point(16, 16), (frogsForward.width() / 2) - 32, frogsForward.height() - 32);
		frogCol.draw(ColorF(Palette::Cyan, 0.25));

		// くるまの移動と当たり判定
		for (auto& i : obs) {
			i.Update();

			// くるま当たり判定
			Rect obstructsCol(i.position.asPoint(), i.texture.width(), i.texture.height());
			if (obstructsCol.intersects(frogCol)) {
				squash.playOneShot();
				frogPos = frogSpawn;
			}

			// 画面外にでたら戻る
			if (i.position.x < -300) i.position.x = lowerRight.x;
			if (i.position.x > 1200) i.position.x = lowerLeft.x;
		}

		// まるたの移動と当たり判定
		for (auto& i : logs) {
			i.Update();

			// まるた当たり判定
			Rect ladderCol(i.position.asPoint(), i.texture.width(), i.texture.height());
			if (ladderCol.intersects(frogCol)) {
				Print << U"on the log";
				frogVel.x += ladderSpeed * Scene::DeltaTime();
			}

			if (i.position.x > 1200) i.position.x = lowerLeft.x;
		}

		// 亀
		for (auto& i : turtles) {
			i.TurtleUpdate();

			// 当たり判定
			Rect turtleCol(i.position.asPoint(), i.texture.width(), i.texture.height());
			if (turtleCol.intersects(frogCol)) {
				Print << U"on the turtle";

				// 沈みかけの亀か
				if (i.IsPlunking()) frogPos = frogSpawn;
			}

			if (i.position.x < -300) i.position.x = lowerRight.x;
		}

		// ピンクの足場
		for (auto i = 0; i < Scene::Width(); i += Tile) {
			Vec2 footing1(i, sceneH);
			Vec2 footing2(i, sceneH / 2);
			footing.draw(i, sceneH);
			footing.draw(i, 640);

			// 当たり判定
			Rect footing1Col(footing1.asPoint(), footing.width(), footing.height());
			Rect footing2Col(footing2.asPoint(), footing.width(), footing.height());
			if (footing1Col.intersects(frogPos) || footing2Col.intersects(frogPos)) {
				Print << U"on the pinkies";
			}

			// ゴールのフレーム
			Rect goalFrame(goalPos.asPoint(), 80, 50);
			if (goalFrame.intersects(frogCol)) {
				Print << U"ゴール";
			}

			goalFrame.draw(ColorF{ Palette::White, .25 });

			// ゴール
			for (auto i = 0; i < 5; i++) {
				const int gx = 118, gy = 253;

				// 当たり判定
				Rect goalsCol[5] = {
					{gx + 160 * i, gy, 60, 60},
				};

				for (auto& j : goalsCol) {
					j.draw(Palette::Red);
				}
			}

			// ---------------描画-------------------

			// くるま
			for (auto& i : obs) {
				i.Draw();
			}

			// まるた
			for (auto& i : logs) {
				i.Draw();
			}

			// 亀
			for (auto& i : turtles) {
				i.TurtleDraw();
			}

			// ゴール
			goal.draw(goalPos);

			if (sw.isRunning() && sw.sF() < 3) {
				Rect(frogPos.asPoint(), Tile, Tile).draw(Palette::Red);
			}
			else {
				// かえる描画
				frogAnims[frogPattern].draw(frogPos);
			}

#if _DEBUG

			font30(Cursor::Pos()).draw(0, 0);
			font30(frogPos).draw(0, font30.fontSize());

#endif // _DEBUG
		}
	}
}
