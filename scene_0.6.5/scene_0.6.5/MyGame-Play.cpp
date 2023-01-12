#include "stdafx.h"

class GameObject {
public:
	Vec2 position;
	Vec2 velocity;
	Texture texture;

	GameObject(Texture $tex, Vec2 $pos, int $speed)	{
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

	int GetTurtleIndex(int _i = 5) {
		int animationSpan = 500; // 1/2秒で切り替え
		int animationIndex = (sw.ms() / animationSpan) % _i;

		return animationIndex;
	}

	/// @return 沈みかけた亀なら true
	bool IsTurtlePlunking() {
		return 2 < GetTurtleIndex();
	}
};

void MyGame::Play() {
	if (SimpleGUI::ButtonAt(U"Title", Vec2{ 0, Scene::Height() })) {
		ChangeScene(&MyGame::Title);
	}

	Scene::Resize(SceneX, SceneY);
	Scene::SetBackground(Color(0, 0, 26));

	TextureRegion frogAnims[8] = {
		frogsForward(0, 0, Tile, Tile),
		frogsForward(Tile, 0, Tile * 2, Tile * 2),
		frogsBack(0, 0, Tile, Tile),
		frogsBack(Tile, 0, Tile * 2, Tile * 2),
		frogsLeft(0, 0, Tile, Tile),
		frogsLeft(Tile, 0, Tile * 2, Tile * 2),
		frogsRight(0, 0, Tile, Tile),
		frogsRight(Tile, 0, Tile * 2, Tile * 2)
	};

	Vec2 respawn(448, 1216);
	Vec2 frogPos(respawn);
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

	Array<GameObject> obstructions;
	obstructions << GameObject(car1, lowerRight, -car1Speed);
	obstructions << GameObject(car1, Vec2(250, lowerRight.y), -car1Speed);
	obstructions << GameObject(car1, Vec2(700, lowerRight.y), -car1Speed);

	obstructions << GameObject(car2, lowerLeft, car2Speed);

	obstructions << GameObject(car3, lowerRight - Vec2(0, Tile * 2), -car3Speed);

	obstructions << GameObject(car4, lowerLeft - Vec2(0, Tile * 2), car4Speed);

	obstructions << GameObject(car5, lowerRight - Vec2(0, Tile * 4), -car5Speed);

	Array<GameObject> logs;
	logs << GameObject(log, upperLeft, ladderSpeed);
	logs << GameObject(log, upperLeft - Vec2(0, Tile), ladderSpeed);
	logs << GameObject(log, upperLeft - Vec2(0, Tile * 3), ladderSpeed);

	Array<Turtle> turtles;
	turtles << Turtle(turtle, upperRight, ladderSpeed);
	turtles << Turtle(turtle, upperRight, ladderSpeed);

	double se = .3;
	squash.setVolume(se);

	while (Update()) {
		frogPos = frogVel;
		auto forwardDown = (KeyUp | KeyW).down(),
			leftDown = (KeyLeft | KeyA).down(),
			backDown = (KeyDown | KeyS).down(),
			rightDown = (KeyRight | KeyD).down();

		// ---------------移動-------------------
		/*if (forwardDown) frogPos.y -= Tile;*/
		if (forwardDown) frogVel.set(0, -Tile);
		else if (backDown) frogPos.y += Tile;
		else if (leftDown) frogPos.x -= Tile - 10;
		else if (rightDown) frogPos.x += Tile - 10;
		frogPos.x >= sceneW ? frogPos.x = sceneW : false;
		frogPos.x <= 0 ? frogPos.x = 0 : false;
		frogPos.y >= sceneH ? frogPos.y = sceneH : false;
		frogPos.y <= 0 ? frogPos.y = 0 : false;

		Rect frogCol(frogPos.asPoint(), frogsForward.width() / 2, frogsForward.height());

		// くるま達の移動と当たり判定
		for (auto& i : obstructions) {
			i.Update();

			// くるま当たり判定
			Rect obstructObjectsCollision(i.position.asPoint(), i.texture.width(), i.texture.height());
			if (obstructObjectsCollision.intersects(frogCol)) {
				squash.playOneShot();
				frogPos = respawn;
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

			//isStepping = false;
			/*
			320 木
			384 亀
			448 木
			512 木
			576 亀
			*/
		}

		for (auto& i : turtles) {
			i.TurtleUpdate();

			// かめ当たり判定
			Rect turtleCol(i.position.asPoint(), i.texture.width(), i.texture.height());
			if (turtleCol.intersects(frogCol)) {
			}
		}

		// ピンクの足場の描画と当たり判定
		for (auto i = 0; i < Scene::Width(); i += Tile) {
			Vec2 footing1(i, sceneH);
			Vec2 footing2(i, 640);
			footing.draw(i, sceneH);
			footing.draw(i, 640);

			// 足場当たり判定
			Rect footing1Col(footing1.asPoint(), footing.width(), footing.height());
			Rect footing2Col(footing2.asPoint(), footing.width(), footing.height());
			if (footing1Col.intersects(frogPos) || footing2Col.intersects(frogPos)) {
				Print << U"on the pinkies";
			}
		}

		// ゴール当たり判定
		//Rect goalFailure(goalPosition.asPoint(), goal.width(), goal.height());

		Rect goalFail(goalPos.asPoint(), 80, 50);
		if (goalFail.intersects(frogCol)) {
			Print << U"ゴール";
		}
		goalFail.draw(ColorF{Palette::White, .25});

		// ゴールの中
		auto goalX = 118, goalY = 253;
		for (auto i = 0; i < 5; i++) {
			Rect goalsCol[5] = {
				{goalX + 160 * i, goalY, 60, 60},
			};

			for (auto& j : goalsCol) {
				j.draw(Palette::Red);
			}
		}
		
		// ---------------描画-------------------
		// くるま
		for (auto& c : obstructions) {
			c.Draw();
		}

		// まるた
		for (auto& l : logs) {
			l.Draw();
		}

		// 亀
		for (auto& t : turtles) {
			t.TurtleDraw();
		}

		// ゴール
		goal.draw(goalPos);


		// かえる描画
		frogAnims[0].draw(frogPos);


#if _DEBUG

		font30(Cursor::Pos()).draw(0, 0);
		font30(frogPos).draw(0, font30.fontSize());

#endif // _DEBUG
	}

}
