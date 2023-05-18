#pragma once

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


