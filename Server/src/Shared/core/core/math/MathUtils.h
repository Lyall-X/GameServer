#pragma once

class MathUtils
{
public:
	static const float PI;
	static const float DEGREES;
	static const float RADIANS;
	static float ToDegree(float radian) { return (DEGREES * radian); }
	static float ToRadian(float degree) { return (RADIANS * degree); }
public:
	static float tan(float f);
	static double tan(double f);

	static float atan2(float y, float x);
	static double atan2(double y, double x);

	static float cos(float f);
	static double cos(double f);

	static float acos(float f);
	static double acos(double f);

	static float sin(float f);
	static double sin(double f);

	static float asin(float f);
	static double asin(double f);

	static float sqrt(float f);
	static double sqrt(double f);

	static float abs(float f);
	static double abs(double f);
public:
	MathUtils(void);
	~MathUtils(void);
};

