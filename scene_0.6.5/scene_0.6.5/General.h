#pragma once
class General
{
public:
	// どこでも使える
	inline static int x;

	inline static int currentScore;
	inline static int highScore;

	inline static float timer;

	inline static void Initialize()
	{
		timer = 0;
		currentScore = 0;
		highScore = 0;
	}
};

