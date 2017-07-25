
template <typename real>
real NtMath<real>::Sin(real value)
{
	return (real)sin((double)value);
}

template <typename real>
real NtMath<real>::Cos(real value)
{
	return (real)cos((double)value);
}

template <typename real>
real NtMath<real>::Tan(real value)
{
	return (real)tan((double)value);
}

template <typename real>
real NtMath<real>::ASin(real value)
{
	if (-(real)1.0 < value)
	{
		if ((real)1.0 > value)
		{
			return asin((double)value)
		}
		else
		{
			return HALF_PI;
		}
	}
	else
	{
		return -HALF_PI;
	}
}

template <typename real>
real NtMath<real>::ACos(real value)
{
	if (-(real)1.0 < value)
	{
		if ((real)1.0 > value)
		{
			return (real)acos((double)value);
		}
		else
		{
			return (real)0.0;
		}
	}
	else
	{
		return PI;
	}
}

template <typename real>
real NtMath<real>::ATan(real value)
{
	return (real)atan((double)value);
}

template <typename real>
real NtMath<real>::ATan2(real yValue, real xValue)
{
	return (real)atan2((double)yValue, (double)xValue);
}

template <typename real>
real NtMath<real>::Angle(ntFloat x, ntFloat y)
{
	ntFloat theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// if x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
		{
			theta += 2.0f * PI; // in [0, 2*pi).
		}
	}
	else
	{
		// Quadrant II or III
		theta = atanf(y / x) + PI; // in [0, 2 * pi).
	}
	return theta;
}

template <typename real>
real NtMath<real>::Floor(real value)
{
	return (real)floor((double)value);
}

template <typename real>
real NtMath<real>::Ceil(real value)
{
	return (real)ceil((double)value);
}

template <typename real>
real NtMath<real>::FMod(real xValue, real yValue)
{
	return (real)fmod((double)xValue, (double)yValue);
}

template <typename real>
real NtMath<real>::Pow(real originValue, real exponent)
{
	return (real)pow((double)originValue, (double)exponent);
}

template <typename real>
real NtMath<real>::Sqr(real value)
{
	return value * value;
}

template <typename real>
real NtMath<real>::Sqrt(real value)
{
	return (real)sqrt((double)value);
}

template <typename real>
real NtMath<real>::InvSqrt(real value)
{
	return (real)(1.0 / (double)sqrt((double)value));
}

template <typename real>
real NtMath<real>::Log(real value)
{
	return (real)log((double)value);
}

template <typename real>
real NtMath<real>::Exp(real value)
{
	return (real)exp((double)value);
}

template <typename real>
real NtMath<real>::Abs(real val)
{
	return abs(val);
}

template <typename real>
bool NtMath<real>::IsZero(real val)
{
	return Abs(val) < NtMath<real>::TOLERANCE;
}

template <typename real>
real NtMath<real>::Min(real a, real b)
{
	return a <= b ? a : b;
}

template <typename real>
real NtMath<real>::Max(real a, real b)
{
	return b > a ? b : a;
}

template <typename real>
real NtMath<real>::Clamp(real current, real min, real max)
{
	return Min(Max(min, current), max);
}
