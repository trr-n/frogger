#pragma once

/// @brief 移動速度の値を管理
class Speeds
{
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
	Speeds(int c0, int c1, int c2, int c3, int c4, int log, int turtle)
	{
		car0 = c0;
		car1 = c1;
		car2 = c2;
		car3 = c3;
		car4 = c4;
		Speeds::log = log;
		Speeds::turtle = turtle;
	}
};

