﻿#pragma once

class Score
{
public:
	int current = 0;

	int jump;
	int nest;
	int clear;
	int hit;

	Score(int jump, int nest, int clear, int hit)
	{
		Score::jump = jump;
		Score::nest = nest;
		Score::clear = clear;
		Score::hit = hit;
		current = 0;
	}

	void Add(int amount)
	{
		current += amount;
	}

	void Final(double time)
	{
		General::FinalPoint = current;
		General::FinalTime = time;
	}

	//int Current()
	//{
		//return current;
	//}
};
