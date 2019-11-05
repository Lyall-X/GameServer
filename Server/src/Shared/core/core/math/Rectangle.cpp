#include "Shared.hpp"

Rectangle::Rectangle(float32 fx, float32 fy, float32 fw, float32 fh):
x(fx),
y(fy),
width(fw),
height(fh)
{

}

Rectangle::Rectangle():
x(0.f),
y(0.f),
width(0.f),
height(0.f)
{

}

bool Rectangle::Intersect(float32 fx, float32 fy)
{
	if (fx < x || fy < y || fx > (x + width) || fy > (y + height))
		return false;

	return true;
}

