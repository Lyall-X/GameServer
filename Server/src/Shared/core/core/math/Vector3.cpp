#include "Shared.hpp"

const Vector3 Vector3::AXIS_X(1, 0, 0);
const Vector3 Vector3::AXIS_Y(0, 1, 0);
const Vector3 Vector3::AXIS_Z(0, 0, 1);

Vector3::Vector3(void):
x(0.f),
y(0.f),
z(0.f)
{
	
}

Vector3::Vector3( const float fx, const float fy, const float fz ):
x(fx),
y(fy),
z(fz)
{

}

Vector3::Vector3( const float other[3] ):
x(other[0]),
y(other[1]),
z(other[2])
{

}

Vector3::Vector3( float* const other ):
x(other[0]),
y(other[1]),
z(other[2])
{

}

Vector3::Vector3( const float scaler ):
x(scaler),
y(scaler),
z(scaler)
{

}

Vector3::Vector3(const Vector3& other):
x(other.x),
y(other.y),
z(other.z)
{

}


Vector3::~Vector3(void)
{
}

Vector3& Vector3::operator=( const Vector3& other )
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

Vector3& Vector3::operator=( const float scaler )
{
	this->x = scaler;
	this->y = scaler;
	this->z = scaler;
	return *this;
}

bool Vector3::operator==( const Vector3& other ) const
{
	return (this->x == other.x && this->y == other.y && this->z == other.z);
}

bool Vector3::operator!=( const Vector3& other ) const
{
	return (this->x != other.x || this->y != other.y || this->z != other.z);
}

Vector3 Vector3::operator+( const Vector3& other ) const
{
	return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3 Vector3::operator-( const Vector3& other ) const
{
	return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}

const Vector3& Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-this->x, -this->y, -this->z);
}

Vector3 Vector3::operator*( const Vector3& other ) const
{
	return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
}

Vector3 Vector3::operator*( const float scaler ) const
{
	return Vector3(this->x * scaler, this->y * scaler, this->z * scaler);
}

Vector3 Vector3::operator/( const Vector3& other ) const
{
	return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
}

Vector3 Vector3::operator/( const float scaler ) const
{
	return Vector3(this->x / scaler, this->y / scaler, this->z / scaler);
}


Vector3& Vector3::operator+=( const Vector3& other )
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vector3& Vector3::operator+=( const float scaler )
{
	this->x += scaler;
	this->y += scaler;
	this->z += scaler;
	return *this;
}

Vector3& Vector3::operator-=( const Vector3& other )
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

Vector3& Vector3::operator-=( const float scaler )
{
	this->x -= scaler;
	this->y -= scaler;
	this->z -= scaler;
	return *this;
}

Vector3& Vector3::operator*=( const Vector3& other )
{
	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;
	return *this;
}

Vector3& Vector3::operator*=( const float scaler )
{
	this->x *= scaler;
	this->y *= scaler;
	this->z *= scaler;
	return *this;
}

Vector3& Vector3::operator/=( const Vector3& other )
{
	this->x /= other.x;
	this->y /= other.y;
	this->z /= other.z;
	return *this;
}

Vector3& Vector3::operator/=( const float scaler )
{
	this->x /= scaler;
	this->y /= scaler;
	this->z /= scaler;
	return *this;
}

float Vector3::length() const
{
	return MathUtils::sqrt((float)(this->x * this->x + this->y * this->y + this->z * this->z));
}

float Vector3::distance( const Vector3& other ) const
{
	return (*this - other).length();
}

float Vector3::dotProduct( const Vector3& other ) const
{
	return (float)(this->x * other.x + this->y * other.y + this->z * other.z);
}

Vector3 Vector3::crossProduct( const Vector3& other ) const
{
	return Vector3((float)(this->y * other.z - this->z * other.y), (float)(this->z * other.x - this->x * other.z), (float)(this->x * other.y - this->y * other.x));
}

void Vector3::normalise()
{
	float fl = this->length();
	if (fl != 0.f)
	{
		fl = 1.f / fl;
		x *= fl;
		y *= fl;
		z *= fl;
	}
}

Vector3 Vector3::midPoint( const Vector3& other ) const
{
	return Vector3((this->x + other.x) * 0.5f, (this->y + other.y) * 0.5f, (this->z + other.z) * 0.5f);
}

std::string Vector3::toString() const
{
	char str[256] = {0};
	sprintf_s(str, 256, "Vector3(%2f, %2f, %2f)", x, y, z);
	return std::string(str);
}

void Vector3::set( float fx, float fy, float fz )
{
	this->x = fx;
	this->y = fy;
	this->z = fz;
}

Vector3::operator float*()
{
	return (float*)this;
}

Vector3::operator const float*()
{
	return (const float*)this;
}

Vector3& Vector3::lerp(const Vector3& v0, const Vector3& v1, float t)
{
	x = (v1.x - v0.x) * t + v0.x;
	y = (v1.y - v0.y) * t + v0.y;
	z = (v1.z - v0.z) * t + v0.z;
	return *this;
}

