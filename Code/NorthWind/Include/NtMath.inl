
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
