#pragma once

/// @brief 巣
class FrogNest
{
public:
	Rect nest; // 巣の位置と大きさと同じ判定を持つ矩形
	Texture sit; // 座っている画像 
	bool isSitting = false; // カエルが座っているか

	FrogNest(void);

	FrogNest(Texture _sit)
	{
		sit = _sit;
	}

	/// @brief カエルが座っていたら描画（IsFrogSits==true）、そうでなかったら、何も描かない
	void Draw()
	{
		/*	if (!isSitting)
			{
				return;
			}*/
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
		return nest.intersects(_playerRect); // ここがtrueならプレイヤーミス
	}
};


