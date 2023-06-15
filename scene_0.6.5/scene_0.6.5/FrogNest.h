#pragma once

/// @brief 巣
class FrogNest
{
public:
	Rect nest; // 巣の位置と大きさと同じ判定を持つ矩形
	Texture sit; // 座っている画像 
	bool isSitting = false; // カエルが座っているか
	inline static bool intoNest = false; // かえるが巣にはいった
	bool intoNestSelf = false;

	FrogNest(void);

	FrogNest(int x, int y, int width, int height, Texture _sit)
	{
		sit = _sit;
		isSitting = false;
		intoNest = false;
		nest.set(x, y, width, height);
	}

	/// @brief カエルが座っていたら描画（IsFrogSits==true）、そうでなかったら、何も描かない
	void Draw()
	{
		if (!isSitting)
		{
			return;
		}
		sit.draw(nest.x, nest.y);
#if _DEBUG
		Rect(64, 64).drawFrame(5, RandomHSV());
#endif
		//sit.draw(Scene::Width() / (double)2, Scene::Height() / (double)2);
	}

	// プレイヤーの判定とnestをチェックしてカエルが座っていなかったら IsFrogSits=trueにして return false で抜ける
	// そうでなかったら、return でtrue を返す（プレイヤーミス）
	bool HitCheck(Rect _playerRect)
	{
		// プレイヤーとあたっていて且つかえるが座っていたらtrue
		bool hit = nest.intersects(_playerRect);

		// かえると当たっていて, すわっていたら
		if (hit && isSitting)
		{
			return true;
		}
		// かえるとあたっていて、すわっていなかったら
		if (hit && !isSitting)
		{
			isSitting = true; // かえるを座らせた
			intoNestSelf = true; // 巣一つ分のフラグ
			intoNest = true; // 巣にはいった
			return false; // ぶつかってないから false
		}
		return hit;
	}
};


