#pragma once

#include "Coordinate.h"
#include "Cardinal.h"

#include <vector>

class CoordinatePath
{
private:
	Coordinate fCoordinate;
	std::vector<Cardinal> fPath;

public:
	CoordinatePath(Coordinate aCoordinate, std::vector<Cardinal> aPath);

	Coordinate & getCoordinate();
	std::vector<Cardinal> & getPath();
};
