#pragma once

class Rectangle
{
public:
	float32 x;
	float32 y;
	float32 width;
	float32 height;
	Rectangle();
	Rectangle(float32 fx, float32 fy, float32 fw, float32 fh);
public:
	bool Intersect(float32 fx, float32 fy);
};