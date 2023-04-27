#pragma once

class GameObject
{
public:
	Vec2 position = { 0, 0 };
	Vec2 velocity = { 0, 0 };
	Texture texture;

	Vec2* playerPosPtr;
	Vec2 noData = { 0, 0 };
	Vec2& playerPos = noData;
	const Vec2 nonData = { 1280, 1280 };
	/// @brief 丸太の左上からズレてる量
	Vec2 shift = nonData;

	GameObject() :playerPosPtr(nullptr), position(0, 0), velocity(0, 0)
	{}

	/// @brief 初期値を指定
	/// @param $tex テクスチャ
	/// @param $pos 座標
	/// @param $speed 移動速度
	GameObject(Texture $tex, Vec2 $pos, int $speed) :playerPosPtr(nullptr), position(0, 0), velocity(0, 0)
	{
		texture = $tex;
		position = $pos;
		velocity.set($speed, 0);
	}

	/// @brief 移動、更新
	void Update()
	{
		Vec2 rideSpeed = velocity;
		position += rideSpeed;
	}

	void Drop()
	{
		playerPos = noData;
	}

	/// @brief オブジェクトに乗った瞬間の処理 / ズレ量の計算
	/// @param _position プレイヤーの座標 ポインター
	void Riding(Vec2* _playerPos)
	{
		playerPosPtr = _playerPos;
	}

	bool IsRiding()
	{
		return shift != nonData;
	}

	/// @brief 描画
	void Draw()
	{
		texture.draw(position);
	}
};

