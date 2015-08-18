
#include "NtCoreLib.h"
#pragma hdrstop


NtPoint::NtPoint()
	: x(0.0f), y(0.0f)
{}

NtPoint::NtPoint(NtFloat pX, NtFloat pY)
	: x(pX), y(pY)
{}

NtPoint::NtPoint(const NtPoint& point)
	: x(point.x), y(point.y)
{}

NtPoint& NtPoint::operator=(const NtPoint& point)
{
	x = point.x;
	y = point.y;
	return *this;
}

NtPoint& NtPoint::operator += (const NtPoint& point)
{
	x += point.x;
	y += point.y;
	return *this;
}

NtPoint& NtPoint::operator -= (const NtPoint& point)
{
	x -= point.x;
	y -= point.y;
	return *this;
}

NtPoint& NtPoint::operator *= (const NtPoint& point)
{
	x *= point.x;
	y *= point.y;
	return *this;
}

NtPoint& NtPoint::operator /= (const NtPoint& point)
{
	x /= point.x;
	y /= point.y;
	return *this;
}

NtPoint& NtPoint::operator *= (NtFloat s)
{
	x *= s;
	y *= s;
	return *this;
}

const NtPoint NtPoint::operator+(const NtPoint& point)
{
	return NtPoint(x + point.x, y + point.y);
}

const NtPoint NtPoint::operator-(const NtPoint& point)
{
	return NtPoint(x - point.x, y - point.y);
}

const NtPoint NtPoint::operator*(const NtPoint& point)
{
	return NtPoint(x * point.x, y * point.y);
}

const NtPoint NtPoint::operator/(const NtPoint& point)
{
	return NtPoint(x / point.x, y / point.y);
}

const NtPoint NtPoint::operator-()
{
	return NtPoint(-x, -y);
}

bool NtPoint::operator==(const NtPoint& point)
{
	return (x == point.x && y == point.y) ? true : false;
}

bool NtPoint::operator!=(const NtPoint& point)
{
	return (x != point.x || y != point.y) ? true : false;
}
