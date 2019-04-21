#include "CoordinatePath.h"

CoordinatePath::CoordinatePath(Coordinate aCoordinate, std::vector<Cardinal> aPath)
{
	fCoordinate = aCoordinate;
	fPath = aPath;
}

Coordinate & CoordinatePath::getCoordinate()
{
	return fCoordinate;
}

std::vector<Cardinal> & CoordinatePath::getPath()
{
	return fPath;
}
