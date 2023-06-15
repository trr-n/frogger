#pragma once
class General
{
public:
	General(void);

	inline static int FinalPoint;
	inline static int FinalTime;
	inline static void SetFinalScores(int point, int time)
	{
		FinalPoint = point;
		FinalTime = time;
	}

	inline static void Initialize()
	{
		FinalPoint = 0;
		FinalTime = 0;
	}

	inline static Font FontSetting(int size, Typeface type = Typeface::Regular, FontStyle style = FontStyle::Default)
	{
		return Font(size, type, style);
	}

	inline static Font FontSetting(int size, String path)
	{
		return Font(size, path);
	}
};

