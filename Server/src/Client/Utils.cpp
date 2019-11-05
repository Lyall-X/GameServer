#include "stdafx.h"

Point2 Utils::Vector2ToPoint2(Vector2 pos, float32 w, float32 h)
{
	Point2 pt = { 0 };
	pt.x = (int32)(-pos.y / h);
	pt.y = (int32)(pos.x / w);
	return pt;
}

Point2 Utils::Vector2ToPoint2(float x, float y, float32 w, float32 h)
{
	Point2 pt = {0};
	pt.x = (int32)(-y / h);
	pt.y = (int32)(x / w);
	return pt;
}

Vector2 Utils::Point2ToVector2(Point2 pt, float32 w, float32 h)
{
	Vector2 pos;

	pos.x = pt.y * w + 0.5f * w;
	pos.y = -pt.x * w - 0.5f * h;
	return pos;
}

Vector2 Utils::Point2ToVector2(int32 x, int32 y, float32 w, float32 h)
{
	Vector2 pos;
	pos.x = y * w + 0.5f * w;
	pos.y = -x * w - 0.5f * h;
	return pos;
}

int8 Utils::CalculateDir(Vector2& dir)
{
	float32 angle = Vector2::Angle(Vector2::AXIS_Y, dir);

	if (angle >= 0.f && angle < 22.5f)
		return D_UP;

	if (angle >= 22.5f && angle < 67.5f)
		return D_UPRIGHT;

	if (angle >= 67.5f && angle < 112.5f)
		return D_RIGHT;

	if (angle >= 112.5f && angle < 157.5f)
		return D_DOWNRIGHT;

	if (angle >= 157.5f && angle <= 180.f)
		return D_DOWN;

	if (angle >= -180.f && angle < -157.5f)
		return D_DOWN;

	if (angle >= -157.5f && angle < -112.5f)
		return D_DOWNLEFT;

	if (angle >= -112.5f && angle < -67.5f)
		return D_LEFT;

	if (angle >= -67.5f && angle < -22.5f)
		return D_UPLEFT;

	if (angle >= -22.5f && angle < 0.f)
		return D_UP;
	return 0;
}

