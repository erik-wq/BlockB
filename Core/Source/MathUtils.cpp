#include "MathUtils.h"

float MathUtils::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}
