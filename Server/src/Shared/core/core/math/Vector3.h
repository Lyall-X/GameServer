#pragma once

class BitStream;

class Vector3
{
public:
	float x,y,z;
public:
	static const Vector3 AXIS_X;
	static const Vector3 AXIS_Y;
	static const Vector3 AXIS_Z;
public:
	Vector3(void);
	Vector3(const float fx, const float fy, const float fz);
	Vector3(const float other[3]);
	Vector3(float* const other);
	Vector3(const float scaler);
	Vector3(const Vector3& other);
	~Vector3(void);

	operator float*();
	operator const float*();

	Vector3& operator = (const Vector3& other);
	Vector3& operator = (const float scaler);
	bool operator ==(const Vector3& other) const;
	bool operator !=(const Vector3& other) const;
	Vector3 operator +(const Vector3& other) const;
	Vector3 operator -(const Vector3& other) const;
	Vector3 operator *(const Vector3& other) const;
	Vector3 operator *(const float scaler) const;
	Vector3 operator /(const Vector3& other) const;
	Vector3 operator /(const float scaler) const;
	const Vector3& operator +() const;
	Vector3 operator -() const;

	Vector3& operator +=(const Vector3& other);
	Vector3& operator +=(const float scaler);

	Vector3& operator -=(const Vector3& other);
	Vector3& operator -=(const float scaler);

	Vector3& operator *=(const Vector3& other);
	Vector3& operator *=(const float scaler);

	Vector3& operator /=(const Vector3& other);
	Vector3& operator /=(const float scaler);

	void set(float fx, float fy, float fz);
	float length() const;

	float distance(const Vector3& other) const;

	float dotProduct(const Vector3& other) const;
	Vector3 crossProduct(const Vector3& other) const;
	void normalise();

	Vector3 midPoint(const Vector3& other) const;
	std::string toString() const;
	Vector3& lerp(const Vector3& v0, const Vector3& v1, float t);
};


