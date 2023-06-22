#pragma once

class Turtle {
	Rect drawregion{ 0, 0, 0, 0 };

public:
	Texture texture;
	Stopwatch sw;
	const int width = 64, height = 64;
	Vec2 position, velocity;

	Vec2* playerPosPtr;
	Vec2  noData = { 0,0 };
	Vec2& playerPos = noData;
	const Vec2 nonData = { 1280, 1280 };
	Vec2 shift = noData;

	Turtle() :playerPosPtr(nullptr), position(0, 0), velocity(0, 0) {}

	Turtle(Texture texture, Vec2 position, int _speed)
	{
		this->texture = texture;
		this->position = position;
		velocity.set(_speed, 0);
		sw.start();
	}

	void Update()
	{
		position += velocity; // *Scene::DeltaTime();
		drawregion.set(GetIndex() * width, 0, width, height);
	}

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
	/// @param plunkingAnim 沈み始めのアニメーションパターン 
	/// @return 沈んでいたら true, 浮いていたら false
	bool IsPlunking(int plunkingAnim = 6)
	{
		return plunkingAnim < GetIndex();
	}
	void Riding(Vec2* _playerPos)
	{
		playerPosPtr = _playerPos;
	}

	bool IsRiding()
	{
		return shift != nonData;
	}
};


