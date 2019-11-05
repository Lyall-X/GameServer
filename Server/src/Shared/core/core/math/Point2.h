#pragma once

struct Point2
{
	int32 x;
	int32 y;

	//bool operator ==(const Point2& other) const;
	//bool operator !=(const Point2& other) const;

	float32 distance(const Point2& other) const;
	float32 length() const;
};