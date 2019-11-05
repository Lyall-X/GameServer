#pragma once

class Vector3;

class Vector4
{
public:
	union {
		struct {
			float r, b, g, a;
		};
		struct {
			float x, y, z, w;
		};
	};
public:
	Vector4(void);
	Vector4(float fx, float fy, float fz, float fw);
	Vector4(const float other[4]);
	Vector4(float* const other);
	Vector4(const float scaler);
	Vector4(const Vector4& other);
	Vector4(const Vector3& other);
	~Vector4(void);

	operator float*();
	operator const float*();

	Vector4& operator = (const Vector4& other);
	Vector4& operator = (const float scaler);
	bool operator ==(const Vector4& other) const;
	bool operator !=(const Vector4& other) const;
	Vector4 operator +(const Vector4& other) const;
	Vector4 operator -(const Vector4& other) const;
	Vector4 operator *(const Vector4& other) const;
	Vector4 operator *(const float scaler) const;
	Vector4 operator /(const Vector4& other) const;
	Vector4 operator /(const float scaler) const;
	const Vector4& operator +() const;
	Vector4 operator -() const;

	Vector4& operator +=(const Vector4& other);
	Vector4& operator +=(const float scaler);

	Vector4& operator -=(const Vector4& other);
	Vector4& operator -=(const float scaler);

	Vector4& operator *=(const Vector4& other);
	Vector4& operator *=(const float scaler);

	Vector4& operator /=(const Vector4& other);
	Vector4& operator /=(const float scaler);

	void set(float fx, float fy, float fz, float fw);
	float length() const;
	float distance(const Vector4& other) const;
	void normalise();
	Vector4& fromColor(unsigned int color);
	unsigned int toColor();
	std::string toString() const;
};

