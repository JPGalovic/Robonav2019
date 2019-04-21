#pragma once

/**
 * Grid, used to define game grid for agent to solve.
 * @version	1.2.1
 * @author	James Galovic - 6819710
 * @date	16-04-2019
 */

#include "Coordinate.h"

#include <string>
#include <vector>

class Grid
{
public:
	class Cell
	{
	private:
		Coordinate fCoordinate;
		Cell * fNorth; Cell * fWest; Cell * fSouth; Cell * fEast;
		std::string fValue;

	public:
		Cell();
		Cell(const std::string aValue, const int aX, const int aY);

		void setValue(const std::string aValue);
		void setNorth(Cell & aCell);
		void setWest(Cell & aCell);
		void setSouth(Cell & aCell);
		void setEast(Cell & aCell);

		void remove();

		const std::string getValue();
		Coordinate getCoordinate();
		const int getX();
		const int getY();

		Cell & getNorth();
		Cell & getWest();
		Cell & getSouth();
		Cell & getEast();

		const bool hasNorth();
		const bool hasWest();
		const bool hasSouth();
		const bool hasEast();
	};

private:
	int fWidth, fHeight;
	Coordinate fAgentStart;
	std::vector<Coordinate> fAgentGoals;

	Cell * fOrigin;

public:
	Grid();
	Grid(const std::string aFileName);
	Grid(const std::string aValue, int aWidth, int aHeight);

	void setCellValue(const std::string aValue, const int aX, const int aY);
	void setWall(const int aX, const int aY, const int aWidth, const int aHeight);

	Cell & getCell(const int aX, const int aY);
	Cell & getCell(Coordinate & aCoordinate);
	const std::string getCellValue(const int aX, const int aY);

	//Utilities
	const int getWidth();
	const int getHeight();

	const int getGoalCount();

	Coordinate getStartingLocation();
	Coordinate getGoalLocation(int i);

	bool goalAt(const Coordinate & aCoordinate);
};