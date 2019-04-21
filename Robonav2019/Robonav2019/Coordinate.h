#pragma once

/**
 * Coordinate, used to define a whole-number coordinate (in terms of x & y)
 * @version v1.2.1
 * @author	James Galovic - 6819710
 * @date	16-04-2019
 */
class Coordinate
{
private:
	int fX, fY;

public:
	Coordinate();
	Coordinate(const int aX, const int aY);

	const int getX();
	const int getY();

	void setCoordinate(const int aX, const int aY);
	void setCoordinate(const Coordinate & aCoordinate);
	void setX(const int aX);
	void setY(const int aY);

	Coordinate getNorth();
	Coordinate getWest();
	Coordinate getSouth();
	Coordinate getEast();

	const bool operator==(const Coordinate & aOther);
	const bool operator!=(const Coordinate & aOther);
};