#include "Shared.hpp"

//bool Point2::operator==(const Point2& other) const
//{
//	return (this->x == other.x && this->y == other.y);
//}
//bool Point2::operator!=(const Point2& other) const
//{
//	return (this->x != other.x || this->y != other.y);
//}
//
//float32 Point2::distance(const Point2& other) const
//{
//	return (*this - other).length();
//}
//
//float Point2::length() const
//{
//	return MathUtils::sqrt((float)(this->x * this->x + this->y * this->y));
//}

float32 Point2::distance(const Point2& other) const
{
	Vector2 v0((float32)x, (float32)y);
	Vector2 v1((float32)other.x, (float32)other.y);
	return v0.distance(v1);
}

float32 Point2::length() const
{
	Vector2 v0((float32)x, (float32)y);
	return v0.length();
}
