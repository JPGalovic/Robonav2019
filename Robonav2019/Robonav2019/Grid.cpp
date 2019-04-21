#include "Grid.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iostream>

Grid::Cell::Cell()
{
	fCoordinate = Coordinate();
	fNorth = nullptr; fWest = nullptr; fSouth = nullptr; fEast = nullptr;
	fValue = std::string();
}

Grid::Cell::Cell(const std::string aValue, const int aX, const int aY)
{
	fCoordinate = Coordinate(aX, aY);
	fNorth = nullptr; fWest = nullptr; fSouth = nullptr; fEast = nullptr;
	fValue = aValue;
}

void Grid::Cell::setValue(const std::string aValue)
{
	fValue = aValue;
}

void Grid::Cell::setNorth(Cell & aCell)
{
	aCell.fSouth = this;
	fNorth = &aCell;
}

void Grid::Cell::setWest(Cell & aCell)
{
	aCell.fEast = this;
	fWest = &aCell;
}

void Grid::Cell::setSouth(Cell & aCell)
{
	aCell.fNorth = this;
	fSouth = &aCell;
}

void Grid::Cell::setEast(Cell & aCell)
{
	aCell.fWest = this;
	fEast = &aCell;
}

void Grid::Cell::remove()
{
	if (hasNorth())
		fNorth->fSouth = nullptr;
	if (hasWest())
		fWest->fEast = nullptr;
	if (hasSouth())
		fSouth->fNorth = nullptr;
	if (hasEast())
		fEast->fWest = nullptr;
	fNorth = nullptr; fWest = nullptr; fSouth = nullptr; fEast = nullptr;
}

const std::string Grid::Cell::getValue()
{
	return fValue;
}

Coordinate Grid::Cell::getCoordinate()
{
	return fCoordinate;
}

const int Grid::Cell::getX()
{
	return fCoordinate.getX();
}

const int Grid::Cell::getY()
{
	return fCoordinate.getY();
}

Grid::Cell & Grid::Cell::getNorth()
{
	return *fNorth;
}

Grid::Cell & Grid::Cell::getWest()
{
	return *fWest;
}

Grid::Cell & Grid::Cell::getSouth()
{
	return *fSouth;
}

Grid::Cell & Grid::Cell::getEast()
{
	return *fEast;
}

const bool Grid::Cell::hasNorth()
{
	return fNorth != nullptr;
}

const bool Grid::Cell::hasWest()
{
	return fWest != nullptr;
}

const bool Grid::Cell::hasSouth()
{
	return fSouth != nullptr;
}

const bool Grid::Cell::hasEast()
{
	return fEast != nullptr;
}

Grid::Grid()
{
	fWidth = 0; fHeight = 0;
	fOrigin = nullptr;
}

Grid::Grid(const std::string aFileName)
{
	std::ifstream lInputFile;
	lInputFile.open(aFileName, std::ios::in);

	if (!lInputFile.is_open() || !lInputFile.good())
	{
		std::string errorStr = "Failed to open file \"";
		errorStr.append(aFileName);
		errorStr.append("\": ");
		errorStr.append(strerror(errno));
		throw std::runtime_error(errorStr);
	}

	bool lDebug = true; // Show debug marks
	int lN = 0; // line Tracking (for formatting)
	std::string lLine;

	while (std::getline(lInputFile, lLine))
	{
		if (lDebug)
			std::cout << lN << ": " << lLine << std::endl;
		lLine.erase(std::remove_if(lLine.begin(), lLine.end(), isspace), lLine.end());
		std::stringstream lLineStream(lLine);
		int lX, lY, lW, lH; // Temp Buffers

		if (lN == 0) // Grid Size
		{
			lLineStream.get(); lLineStream >> lH;
			lLineStream.get(); lLineStream >> lW;
			fWidth = lW; fHeight = lH;

			Cell ** lCellGrid;
			lCellGrid = new Cell*[lW];
			for (int i = 0; i < lW; i++)
				lCellGrid[i] = new Cell[lH];

			for (int j = 0; j < lH; j++)
			{
				for (int i = 0; i < lW; i++)
				{
					lCellGrid[i][j] = Cell("CLR", i, j);

					if (i > 0)
						lCellGrid[i][j].setWest(lCellGrid[i - 1][j]);
					if (j > 0)
						lCellGrid[i][j].setNorth(lCellGrid[i][j - 1]);
				}
			}

			fOrigin = &lCellGrid[0][0];

			if (lDebug)
				std::cout << "Grid size read as: [" << lH << ", " << lW << "]." << std::endl;
		}
		else if (lN == 1) // Agent Start Position
		{
			lLineStream.get(); lLineStream >> lX;
			lLineStream.get(); lLineStream >> lY;

			fAgentStart = Coordinate(lX, lY);

			if (lDebug)
				std::cout << "Agent start position read as: (" << lX << ", " << lY << ")." << std::endl;
		}
		else if (lN == 2) // Agent Goals
		{
			std::vector<int> lValues;
			std::vector<std::string> lTokenStrings;
			std::string lToken;
			while (std::getline(lLineStream, lToken, '|'))
				lTokenStrings.push_back(lToken);

			for (int i = 0; i < lTokenStrings.size(); i++)
			{
				std::stringstream lTokenStream(lTokenStrings[i]);
				int lA, lB;

				lTokenStream.get(); lTokenStream >> lA;
				lTokenStream.get(); lTokenStream >> lB;
				
				fAgentGoals.push_back(Coordinate(lA, lB));
			}

			if (lDebug)
			{
				std::cout << "Read " << fAgentGoals.size() << " goal states: ";
				for (int j = 0; j < fAgentGoals.size(); j++)
				{
					if (j > 0)
						std::cout << " | ";
					std::cout << "(" << fAgentGoals[j].getX() << ", " << fAgentGoals[j].getY() << ")";
				}
				std::cout << "." << std::endl;
			}
		}
		else
		{
			lLineStream.get(); lLineStream >> lX;
			lLineStream.get(); lLineStream >> lY;
			lLineStream.get(); lLineStream >> lW;
			lLineStream.get(); lLineStream >> lH;

			setWall(lX, lY, lW, lH);
			if (lDebug)
				std::cout << "Wall location read as: (" << lX << ',' << lY << "," << lW << ',' << lH << ")" << std::endl;
		}
		lN++;
	}
}

Grid::Grid(const std::string aValue, int aWidth, int aHeight)
{
	fWidth = aWidth; fHeight = aHeight;

	Cell ** lCellGrid;
	lCellGrid = new Cell*[aWidth];
	for (int i = 0; i < aWidth; i++)
		lCellGrid[i] = new Cell[aHeight];

	for (int j = 0; j < aHeight; j++)
	{
		for (int i = 0; i < aWidth; i++)
		{
			lCellGrid[i][j] = Cell(aValue, i, j);

			if (i > 0)
				lCellGrid[i][j].setWest(lCellGrid[i - 1][j]);
			if (j > 0)
				lCellGrid[i][j].setNorth(lCellGrid[i][j - 1]);
		}
	}

	fOrigin = &lCellGrid[0][0];
}

void Grid::setCellValue(const std::string aValue, const int aX, const int aY)
{
	getCell(aX, aY).setValue(aValue);
}

void Grid::setWall(const int aX, const int aY, const int aWidth, const int aHeight)
{
	for (int j = 0; j < aHeight; j++)
		for (int i = 0; i < aWidth; i++)
			setCellValue("WAL", aX + i, aY + j);
}

Grid::Cell & Grid::getCell(const int aX, const int aY)
{
	if (aX > fWidth || aY > fHeight)
		throw std::range_error("Cell coordinate out of range.");

	Cell * lCell = fOrigin;

	while (lCell->getX() != aX)
		lCell = (Cell *)&lCell->getEast();
	while (lCell->getY() != aY)
		lCell = (Cell *)&lCell->getSouth();

	return *lCell;
}

Grid::Cell & Grid::getCell(Coordinate & aCoordinate)
{
	return getCell(aCoordinate.getX(), aCoordinate.getY());
}

const std::string Grid::getCellValue(const int aX, const int aY)
{
	return getCell(aX , aY).getValue();
}

const int Grid::getWidth()
{
	return fWidth;
}

const int Grid::getHeight()
{
	return fHeight;
}

const int Grid::getGoalCount()
{
	return fAgentGoals.size();
}

Coordinate Grid::getStartingLocation()
{
	return fAgentStart;
}

Coordinate Grid::getGoalLocation(int i)
{
	return fAgentGoals[i];
}

bool Grid::goalAt(const Coordinate & aCoordinate)
{
	for (int i = 0; i < fAgentGoals.size(); i++)
		if (fAgentGoals[i] == aCoordinate)
			return true;
	return false;
}
