#include "Coordinate.h"

Coordinate::Coordinate()
{
	fX = 0;
	fY = 0;
}

Coordinate::Coordinate(const int aX, const int aY)
{
	fX = aX;
	fY = aY;
}

const int Coordinate::getX()
{
	return fX;
}

const int Coordinate::getY()
{
	return fY;
}

void Coordinate::setCoordinate(const int aX, const int aY)
{
	fX = aX;
	fY = aY;
}

void Coordinate::setCoordinate(const Coordinate & aCoordinate)
{
	fX = aCoordinate.fX;
	fY = aCoordinate.fY;
}

void Coordinate::setX(const int aX)
{
	fX = aX;
}

void Coordinate::setY(const int aY)
{
	fY = aY;
}

Coordinate Coordinate::getNorth()
{
	return Coordinate(fX, fY - 1);
}

Coordinate Coordinate::getWest()
{
	return Coordinate(fX - 1, fY);
}

Coordinate Coordinate::getSouth()
{
	return Coordinate(fX, fY + 1);
}

Coordinate Coordinate::getEast()
{
	return Coordinate(fX + 1, fY);
}

const bool Coordinate::operator==(const Coordinate & aOther)
{
	return fX == aOther.fX && fY == aOther.fY;
}

const bool Coordinate::operator!=(const Coordinate & aOther)
{
	return !(*this == aOther);
}
