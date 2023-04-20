#pragma once

/// @brief オブジェクトの生成座標を管理
class SpawnPosition
{
public:
	Point leftUp, leftBottom, rightUp, rightBottom;

	SpawnPosition() {
		leftUp = Point(0, 0);
		leftBottom = Point(0, 0);
		rightUp = Point(0, 0);
		rightBottom = Point(0, 0);
	}

	/// @brief 上下レーンの障害物の生成座標を指定
	/// @param _leftUp 上段の左側
	/// @param _leftBottom 下段の左側
	/// @param _rightUp 上段の右側
	/// @param _rightBottom 下段の右側
	SpawnPosition(Point _leftUp, Point _leftBottom, Point _rightUp, Point _rightBottom)
	{
		leftUp.set(_leftUp);
		leftBottom.set(_leftBottom);
		rightUp.set(_rightUp);
		rightBottom.set(_rightBottom);
	}
};

