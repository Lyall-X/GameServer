#pragma once

class BinaryStream;


class Vector2
{
public:
	static const Vector2 AXIS_X;
	static const Vector2 AXIS_Y;
public:
	float x, y;
public:
	Vector2(void);
	Vector2(const float fx, const float fy);
	Vector2(const float other[2]);
	Vector2(float* const other);
	Vector2(const float scaler);
	Vector2(const Vector2& other);
	~Vector2(void);

	operator float*();
	operator const float*();

	Vector2& operator = (const Vector2& other);
	Vector2& operator = (const float scaler);
	bool operator ==(const Vector2& other) const;
	bool operator !=(const Vector2& other) const;
	Vector2 operator +(const Vector2& other) const;
	Vector2 operator -(const Vector2& other) const;
	Vector2 operator *(const Vector2& other) const;
	Vector2 operator *(const float scaler) const;
	Vector2 operator /(const Vector2& other) const;
	Vector2 operator /(const float scaler) const;
	const Vector2& operator +() const;
	Vector2 operator -() const;

	Vector2& operator +=(const Vector2& other);
	Vector2& operator +=(const float scaler);

	Vector2& operator -=(const Vector2& other);
	Vector2& operator -=(const float scaler);

	Vector2& operator *=(const Vector2& other);
	Vector2& operator *=(const float scaler);

	Vector2& operator /=(const Vector2& other);
	Vector2& operator /=(const float scaler);

	void set(float fx, float fy);
	float length() const;

	float distance(const Vector2& other) const;

	float dotProduct(const Vector2& other) const;
	void normalise();

	Vector2 midPoint(const Vector2& other) const;
	std::string toString() const;
	Vector2& lerp(const Vector2& v0, const Vector2& v1, float t);
	static float Distance(const Vector2& v0, const Vector2& v1);
	static float Angle(const Vector2& v0, const Vector2& v1);
};

