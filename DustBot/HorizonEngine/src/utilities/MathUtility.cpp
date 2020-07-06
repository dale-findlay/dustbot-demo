#include "MathUtility.h"

float Horizon::MathUtility::ClampF(float f, float min, float max)
{
	float result = f;

	if (result < min)
	{
		result = min;
	}

	if (result > max)
	{
		result = max;
	}

	return result;
}

int Horizon::MathUtility::Clamp(int i, int min, int max)
{
	int result = i;

	if (result < min)
	{
		result = min;
	}

	if (result > max)
	{
		result = max;
	}

	return result;
}
