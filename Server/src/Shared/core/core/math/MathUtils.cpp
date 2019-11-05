#include "Shared.h"
#include "MathUtils.h"
#include <math.h>

const float MathUtils::PI = 3.141592653589793f;
const float MathUtils::DEGREES = 180.0f / 3.141592653589793f;
const float MathUtils::RADIANS = 3.141592653f / 180.0f;

MathUtils::MathUtils(void)
{
}

MathUtils::~MathUtils(void)
{
}

float MathUtils::tan( float f )
{
	return ::tanf(f);
}

double MathUtils::tan( double f )
{
	return ::tan(f);
}

float MathUtils::atan2( float y, float x )
{
	return ::atan2f(y, x);
}

double MathUtils::atan2( double y, double x )
{
	return ::atan2(y, x);
}

float MathUtils::cos( float f )
{
	return ::cosf(f);
}

double MathUtils::cos( double f )
{
	return ::cos(f);
}

float MathUtils::acos( float f )
{
	return ::acosf(f);
}

double MathUtils::acos( double f )
{
	return ::acos(f);
}

float MathUtils::sin( float f )
{
	return ::sinf(f);
}

double MathUtils::sin( double f )
{
	return ::sin(f);
}

float MathUtils::asin( float f )
{
	return ::asinf(f);
}

double MathUtils::asin( double f )
{
	return ::asin(f);
}

float MathUtils::sqrt( float f )
{
	return ::sqrtf(f);
}

double MathUtils::sqrt( double f )
{
	return ::sqrt(f);
}

float MathUtils::abs( float f )
{
	return ::fabsf(f);
}

double MathUtils::abs( double f )
{
	return ::fabs(f);
}


