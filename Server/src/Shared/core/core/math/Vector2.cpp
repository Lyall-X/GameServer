#include "Shared.hpp"

const Vector2 Vector2::AXIS_X(1, 0);
const Vector2 Vector2::AXIS_Y(0, 1);

Vector2::Vector2(void) :
	x(0.f),
	y(0.f)
{

}

Vector2::Vector2(const float fx, const float fy) :
	x(fx),
	y(fy)
{

}

Vector2::Vector2(const float other[2]) :
	x(other[0]),
	y(other[1])
{

}

Vector2::Vector2(float* const other) :
	x(other[0]),
	y(other[1])
{

}

Vector2::Vector2(const float scaler) :
	x(scaler),
	y(scaler)
{

}

Vector2::Vector2(const Vector2& other) :
	x(other.x),
	y(other.y)
{

}


Vector2::~Vector2(void)
{
}

Vector2& Vector2::operator=(const Vector2& other)
{
	this->x = other.x;
	this->y = other.y;
	return *this;
}

Vector2& Vector2::operator=(const float scaler)
{
	this->x = scaler;
	this->y = scaler;
	return *this;
}

bool Vector2::operator==(const Vector2& other) const
{
	return (this->x == other.x && this->y == other.y);
}

bool Vector2::operator!=(const Vector2& other) const
{
	return (this->x != other.x || this->y != other.y);
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(this->x - other.x, this->y - other.y);
}

const Vector2& Vector2::operator+() const
{
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-this->x, -this->y);
}

Vector2 Vector2::operator*(const Vector2& other) const
{
	return Vector2(this->x * other.x, this->y * other.y);
}

Vector2 Vector2::operator*(const float scaler) const
{
	return Vector2(this->x * scaler, this->y * scaler);
}

Vector2 Vector2::operator/(const Vector2& other) const
{
	return Vector2(this->x / other.x, this->y / other.y);
}

Vector2 Vector2::operator/(const float scaler) const
{
	return Vector2(this->x / scaler, this->y / scaler);
}


Vector2& Vector2::operator+=(const Vector2& other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vector2& Vector2::operator+=(const float scaler)
{
	this->x += scaler;
	this->y += scaler;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}

Vector2& Vector2::operator-=(const float scaler)
{
	this->x -= scaler;
	this->y -= scaler;
	return *this;
}

Vector2& Vector2::operator*=(const Vector2& other)
{
	this->x *= other.x;
	this->y *= other.y;
	return *this;
}

Vector2& Vector2::operator*=(const float scaler)
{
	this->x *= scaler;
	this->y *= scaler;
	return *this;
}

Vector2& Vector2::operator/=(const Vector2& other)
{
	this->x /= other.x;
	this->y /= other.y;
	return *this;
}

Vector2& Vector2::operator/=(const float scaler)
{
	this->x /= scaler;
	this->y /= scaler;
	return *this;
}

float Vector2::length() const
{
	return MathUtils::sqrt((float)(this->x * this->x + this->y * this->y));
}

float Vector2::distance(const Vector2& other) const
{
	return (*this - other).length();
}

float Vector2::dotProduct(const Vector2& other) const
{
	return (float)(this->x * other.x + this->y * other.y);
}


void Vector2::normalise()
{
	float fl = this->length();
	if (fl != 0.f)
	{
		fl = 1.f / fl;
		x *= fl;
		y *= fl;
	}
}

Vector2 Vector2::midPoint(const Vector2& other) const
{
	return Vector2((this->x + other.x) * 0.5f, (this->y + other.y) * 0.5f);
}

std::string Vector2::toString() const
{
	char str[256] = { 0 };
	sprintf_s(str, 256, "Vector3(%2f, %2f)", x, y);
	return std::string(str);
}

void Vector2::set(float fx, float fy)
{
	this->x = fx;
	this->y = fy;
}

Vector2::operator float*()
{
	return (float*)this;
}

Vector2::operator const float*()
{
	return (const float*)this;
}

Vector2& Vector2::lerp(const Vector2& v0, const Vector2& v1, float t)
{
	x = (v1.x - v0.x) * t + v0.x;
	y = (v1.y - v0.y) * t + v0.y;
	return *this;
}

float Vector2::Distance(const Vector2& v0, const Vector2& v1)
{
	return (v0 - v1).length();
}

float Vector2::Angle(const Vector2& v0, const Vector2& v1)
{
	float sin = v0.x * v1.y - v1.x * v0.y;
	float cos = v0.x * v1.x + v0.y * v1.y;

	return MathUtils::atan2(sin, cos) * MathUtils::DEGREES;
}

