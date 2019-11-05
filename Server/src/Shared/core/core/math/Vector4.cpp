#include "Shared.hpp"

Vector4::Vector4(void):
x(0.f),
y(0.f),
z(0.f),
w(0.f)
{
}

Vector4::Vector4( float fx, float fy, float fz, float fw ):
x(fx),
y(fy),
z(fz),
w(fw)
{

}

Vector4::Vector4( const float other[4] ):
x(other[0]),
y(other[1]),
z(other[2]),
w(other[3])
{

}

Vector4::Vector4( float* const other ):
x(other[0]),
y(other[1]),
z(other[2]),
w(other[3])
{

}

Vector4::Vector4( const float scaler ):
x(scaler),
y(scaler),
z(scaler),
w(scaler)
{

}

Vector4::Vector4( const Vector4& other ):
x(other.x),
y(other.y),
z(other.z),
w(other.w)
{

}

Vector4::Vector4( const Vector3& other ):
x(other.x),
y(other.y),
z(other.z),
w(1.f)
{

}


Vector4::~Vector4(void)
{
}

Vector4::operator float*()
{
	return (float*)this;
}

Vector4::operator const float*()
{
	return (const float*)this;
}

Vector4& Vector4::operator=( const Vector4& other )
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

Vector4& Vector4::operator=( const float scaler )
{
	x = scaler;
	y = scaler;
	z = scaler;
	w = scaler;
	return *this;
}

bool Vector4::operator==( const Vector4& other ) const
{
	return (!(x != other.x || y != other.y || z != other.z || w != other.w));
}

bool Vector4::operator!=( const Vector4& other ) const
{
	return (x != other.x || y != other.y || z != other.z || w != other.w);
}

Vector4 Vector4::operator+( const Vector4& other ) const
{
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

const Vector4& Vector4::operator+() const
{
	return *this;
}

Vector4 Vector4::operator-( const Vector4& other ) const
{
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator*( const Vector4& other ) const
{
	return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vector4 Vector4::operator*( const float scaler ) const
{
	return Vector4(x * scaler, y * scaler, z * scaler, w * scaler);
}

Vector4 Vector4::operator/( const Vector4& other ) const
{
	return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
}

Vector4 Vector4::operator/( const float scaler ) const
{
	return Vector4(x / scaler, y / scaler, z / scaler, w / scaler);
}

Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

Vector4& Vector4::operator+=( const Vector4& other )
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;

	return *this;
}

Vector4& Vector4::operator+=( const float scaler )
{
	x += scaler;
	y += scaler;
	z += scaler;
	w += scaler;

	return *this;
}

Vector4& Vector4::operator-=( const Vector4& other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;

	return *this;
}

Vector4& Vector4::operator-=( const float scaler )
{
	x -= scaler;
	y -= scaler;
	z -= scaler;
	w -= scaler;

	return *this;
}

Vector4& Vector4::operator*=( const Vector4& other )
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;

	return *this;
}

Vector4& Vector4::operator*=( const float scaler )
{
	x *= scaler;
	y *= scaler;
	z *= scaler;
	w *= scaler;

	return *this;
}

Vector4& Vector4::operator/=( const Vector4& other )
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;

	return *this;
}

Vector4& Vector4::operator/=( const float scaler )
{
	x /= scaler;
	y /= scaler;
	z /= scaler;
	w /= scaler;

	return *this;
}

void Vector4::set( float fx, float fy, float fz, float fw )
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

float Vector4::length() const
{
	return MathUtils::sqrt(x * x + y * y + z * z + w * w);
}

float Vector4::distance( const Vector4& other ) const
{
	float dx = other.x - x;
	float dy = other.y - y;
	float dz = other.z - z;
	float dw = other.w - w;

	return MathUtils::sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

void Vector4::normalise()
{
	float fl = this->length();
	if (fl != 0.f)
	{
		fl = 1.f / fl;
		x *= fl;
		y *= fl;
		z *= fl;
		w *= fl;
	}
}

std::string Vector4::toString() const
{
	char str[256] = {0};
	sprintf_s(str, 256, "Vector4(%2f, %2f, %2f, %2f)", x, y, z, w);
	return std::string(str);
}

Vector4& Vector4::fromColor( unsigned int color )
{
	w = (color >> 24) / 255.f;
	x = ((color & 0x00ff0000) >> 16) / 255.f;
	y = ((color & 0x0000ff00) >> 8) / 255.f;
	z = (color & 0x000000ff) / 255.f;
	return *this;
}

unsigned int Vector4::toColor()
{
	return ((int)(w * 255.f) << 24) | ((int)(x * 255.f) << 16) | ((int)(y * 255.f) << 8) | (int)(z * 255.f); 
}
