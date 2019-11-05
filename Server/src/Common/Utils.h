#pragma once

class Utils
{
public:
	static Point2 Vector2ToPoint2(Vector2 pos, float32 w, float32 h);
	static Point2 Vector2ToPoint2(float x, float y, float32 w, float32 h);

	static Vector2 Point2ToVector2(Point2 pt, float32 w, float32 h);
	static Vector2 Point2ToVector2(int32 x, int32 y, float32 w, float32 h);

	static int8 CalculateDir(Vector2& dir);
};											  