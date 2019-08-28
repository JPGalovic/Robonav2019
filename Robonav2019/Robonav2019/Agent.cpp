#include "Agent.h"
#include "CoordinatePath.h"
#include "Printers.h"

#include <time.h>
#include <iostream>
#include <queue>
#include <cstdlib>
#include <thread>
#include <mutex>

std::mutex gMtx; // Mutex for thread locking

// Agent Constructor
Agent::Agent(Grid & aGrid)
{
	fLocation = aGrid.getStartingLocation();
	fGrid = &aGrid;
}

void Agent::resetPosition()
{
	fLocation = fGrid->getStartingLocation();
}

bool Agent::move(Cardinal aDirection)
{
	Grid::Cell * lCell = &fGrid->getCell(fLocation);

	switch (aDirection)
	{
	case NORTH:
		if(lCell->hasNorth())
			return moveTo(lCell->getNorth());
		break;
	case WEST:
		if (lCell->hasWest())
			return moveTo(lCell->getWest());
		break;
	case SOUTH:
		if (lCell->hasSouth())
			return moveTo(lCell->getSouth());
		break;
	case EAST:
		if (lCell->hasEast())
			return moveTo(lCell->getEast());
		break;
	}
	return false;
}

bool Agent::moveTo(Grid::Cell & aCell)
{
	if (aCell.getValue() == "CLR")
	{
		fLocation.setCoordinate(aCell.getCoordinate());
		
		print(*fGrid, *this);
		//system("Pause");

		return true;
	}
	return false;
}

Coordinate Agent::getCoordinate()
{
	return fLocation;
}

bool Agent::validMovement(Cardinal aDirection)
{
	Grid::Cell * lCell = &fGrid->getCell(fLocation);
	switch (aDirection)
	{
	case NORTH:
		if (lCell->hasNorth())
			return true;
		break;
	case WEST:
		if (lCell->hasWest())
			return true;
		break;
	case SOUTH:
		if (lCell->hasSouth())
			return true;
		break;
	case EAST:
		if (lCell->hasEast())
			return true;
		break;
	}
	return false;
}

bool Agent::atGoal()
{
	return fGrid->goalAt(fLocation);
}

bool Agent::isAt(int aX, int aY)
{
	return aX == fLocation.getX() && aY == fLocation.getY();
}

/**
 * Depth-First Search Algorithem.
 * Select one option, try it, go back when there are no more options.
 */
std::vector<Cardinal> DFS(Agent & aAgent)
{
	std::vector<Coordinate> lVisited;
	lVisited.push_back(aAgent.getCoordinate()); // Add inital location to visited list.
	std::vector<Cardinal> lResult = DFS(aAgent, lVisited, NONE);
	return lResult;
}

/**
 * Depth-First Search Aux Algorithem.
 * Selects one option, try it, go back when there are no more opions. do not visit node if node has been visited prior.
 */
 std::vector<Cardinal> DFS(Agent & aAgent, std::vector<Coordinate> & aVisited, Cardinal aDirection)
 {
	 std::vector<Cardinal> lMovementList;
	 if (aAgent.atGoal())
		 return lMovementList;

	 //Direction Bias North, West, South, East
	 for (int i = 0; i < 4; i++)
	 {
		 Coordinate lCheckCoordinate;
		 Cardinal lMoveDirection;
		 switch (i)
		 {
		 case 0:
			 lCheckCoordinate = aAgent.getCoordinate().getNorth();
			 lMoveDirection = NORTH;
			 break;
		 case 1:
			 lCheckCoordinate = aAgent.getCoordinate().getWest();
			 lMoveDirection = WEST;
			 break;
		 case 2:
			 lCheckCoordinate = aAgent.getCoordinate().getSouth();
			 lMoveDirection = SOUTH;
			 break;
		 case 3:
			 lCheckCoordinate = aAgent.getCoordinate().getEast();
			 lMoveDirection = EAST;
			 break;
		 }

		 bool lVisited = false;
		 for (int j = 0; j < aVisited.size(); j++)
		 {
			 if (aVisited[j] == lCheckCoordinate)
			 {
				 lVisited = true;
				 break;
			 }
		 }
		 if (!lVisited)
		 {
			 aVisited.push_back(lCheckCoordinate);
			 if (aAgent.move(lMoveDirection))
			 {
				 lMovementList.push_back(lMoveDirection);
				 std::vector<Cardinal> lTemp = DFS(aAgent, aVisited, lMoveDirection);
				 for (int k = 0; k < lTemp.size(); k++)
				 {
					 lMovementList.push_back(lTemp[k]);
				 }
			 }

			 if (aAgent.atGoal())
				 return lMovementList;
		 }
	 }
	 // Go Back
	 switch (aDirection)
	 {
	 case NORTH:
		 aAgent.move(SOUTH);
		 lMovementList.push_back(SOUTH);
		 break;
	 case WEST:
		 aAgent.move(EAST);
		 lMovementList.push_back(EAST);
		 break;
	 case SOUTH:
		 aAgent.move(NORTH);
		 lMovementList.push_back(NORTH);
		 break;
	 case EAST:
		 aAgent.move(WEST);
		 lMovementList.push_back(WEST);
		 break;
	 }
	 return lMovementList;
 }

/**
 * Breadth-First Search Algorithem.
 * Expand all options, one level at a time.
 */
std::vector<Cardinal> BFS(Agent & aAgent)
{
	std::vector<CoordinatePath> lDetermined; // Set of determined nodes, contains path to given node from Agent starting point.
	std::queue<Coordinate> lNodeQueue; // Queue of nodes left to assess.

	lDetermined.push_back(CoordinatePath(aAgent.getCoordinate(), std::vector<Cardinal>()));

	if (aAgent.validMovement(NORTH))
	{
		std::vector<Cardinal> lPathNorth;
		lPathNorth.push_back(NORTH);
		Coordinate lCoordinateNorth = aAgent.getCoordinate().getNorth();

		// Check if determined node has been visited
		bool lVisited = false;
		for (int i = 0; i < lDetermined.size(); i++)
			if (lCoordinateNorth == lDetermined[i].getCoordinate())
				lVisited = true;

		if (!lVisited)
		{
			// if not visited add node to queue and determined list.
			lNodeQueue.push(lCoordinateNorth);
			lDetermined.push_back(CoordinatePath(lCoordinateNorth, lPathNorth));
		}
	}

	if (aAgent.validMovement(WEST))
	{
		std::vector<Cardinal> lPathTemp;
		lPathTemp.push_back(WEST);
		Coordinate lCoordinate = aAgent.getCoordinate().getWest();

		// Check if determined node has been visited
		bool lVisited = false;
		for (int i = 0; i < lDetermined.size(); i++)
			if (lCoordinate == lDetermined[i].getCoordinate())
				lVisited = true;

		if (!lVisited)
		{
			// if not visited add node to queue and determined list.
			lNodeQueue.push(lCoordinate);
			lDetermined.push_back(CoordinatePath(lCoordinate, lPathTemp));
		}
	}

	if (aAgent.validMovement(SOUTH))
	{
		std::vector<Cardinal> lPathTemp;
		lPathTemp.push_back(SOUTH);
		Coordinate lCoordinate = aAgent.getCoordinate().getSouth();

		// Check if determined node has been visited
		bool lVisited = false;
		for (int i = 0; i < lDetermined.size(); i++)
			if (lCoordinate == lDetermined[i].getCoordinate())
				lVisited = true;

		if (!lVisited)
		{
			// if not visited add node to queue and determined list.
			lNodeQueue.push(lCoordinate);
			lDetermined.push_back(CoordinatePath(lCoordinate, lPathTemp));
		}
	}

	if (aAgent.validMovement(EAST))
	{
		std::vector<Cardinal> lPathTemp;
		lPathTemp.push_back(EAST);
		Coordinate lCoordinate = aAgent.getCoordinate().getEast();

		// Check if determined node has been visited
		bool lVisited = false;
		for (int i = 0; i < lDetermined.size(); i++)
			if (lCoordinate == lDetermined[i].getCoordinate())
				lVisited = true;

		if (!lVisited)
		{
			// if not visited add node to queue and determined list.
			lNodeQueue.push(lCoordinate);
			lDetermined.push_back(CoordinatePath(lCoordinate, lPathTemp));
		}
	}
	
	while (!lNodeQueue.empty())
	{
		std::vector<Cardinal> lPath; // Path, used for backtracking.
		// Get path to next node.
		Coordinate lNext = lNodeQueue.front();
		lNodeQueue.pop();
		for (int i = 0; i < lDetermined.size(); i++)
			if (lDetermined[i].getCoordinate() == lNext)
				lPath = lDetermined[i].getPath();

		// Move to next node.
		for (int i = 0; i < lPath.size(); i++)
			aAgent.move(lPath[i]); 

		// Check if at goal.
		if (aAgent.atGoal())
			return lPath;

		// Load connected nodes into queue.
		if (aAgent.validMovement(NORTH))
		{
			std::vector<Cardinal> lPathNorth = lPath;
			lPathNorth.push_back(NORTH);
			Coordinate lCoordinateNorth = aAgent.getCoordinate().getNorth();

			// Check if determined node has been visited
			bool lVisited = false;
			for (int i = 0; i < lDetermined.size(); i++)
			{
				if (lCoordinateNorth == lDetermined[i].getCoordinate())
				{
					lVisited = true;
					break;
				}
			}

			if (!lVisited)
			{
				// if not visited add node to queue and determined list.
				lNodeQueue.push(lCoordinateNorth);
				lDetermined.push_back(CoordinatePath(lCoordinateNorth, lPathNorth));
			}
		}

		if (aAgent.validMovement(WEST))
		{
			std::vector<Cardinal> lPathTemp = lPath;
			lPathTemp.push_back(WEST);
			Coordinate lCoordinate = aAgent.getCoordinate().getWest();

			// Check if determined node has been visited
			bool lVisited = false;
			for (int i = 0; i < lDetermined.size(); i++)
				if (lCoordinate == lDetermined[i].getCoordinate())
					lVisited = true;

			if (!lVisited)
			{
				// if not visited add node to queue and determined list.
				lNodeQueue.push(lCoordinate);
				lDetermined.push_back(CoordinatePath(lCoordinate, lPathTemp));
			}
		}

		if (aAgent.validMovement(SOUTH))
		{
			std::vector<Cardinal> lPathTemp = lPath;
			lPathTemp.push_back(SOUTH);
			Coordinate lCoordinate = aAgent.getCoordinate().getSouth();

			// Check if determined node has been visited
			bool lVisited = false;
			for (int i = 0; i < lDetermined.size(); i++)
				if (lCoordinate == lDetermined[i].getCoordinate())
					lVisited = true;

			if (!lVisited)
			{
				// if not visited add node to queue and determined list.
				lNodeQueue.push(lCoordinate);
				lDetermined.push_back(CoordinatePath(lCoordinate, lPathTemp));
			}
		}

		if (aAgent.validMovement(EAST))
		{
			std::vector<Cardinal> lPathTemp = lPath;
			lPathTemp.push_back(EAST);
			Coordinate lCoordinate = aAgent.getCoordinate().getEast();

			// Check if determined node has been visited
			bool lVisited = false;
			for (int i = 0; i < lDetermined.size(); i++)
				if (lCoordinate == lDetermined[i].getCoordinate())
					lVisited = true;

			if (!lVisited)
			{
				// if not visited add node to queue and determined list.
				lNodeQueue.push(lCoordinate);
				lDetermined.push_back(CoordinatePath(lCoordinate, lPathTemp));
			}
		}
		
		// Return to starting point.
		for (int i = lPath.size() - 1; i >= 0; i--)
		{
			switch (lPath[i])
			{
			case NORTH:
				aAgent.move(SOUTH);
				break;
			case WEST:
				aAgent.move(EAST);
				break;
			case SOUTH:
				aAgent.move(NORTH);
				break;
			case EAST:
				aAgent.move(WEST);
				break;
			}
		}
	}

	return std::vector<Cardinal>();
}

/**
 * Custom Search Method 1.
 * An uninformed method to find a path to reach the goal.
 */
std::vector<Cardinal> CUS1(Agent & aAgent)
{
	std::vector<Cardinal> lMovementList;
	srand(time(NULL));

	/**
	 * Algorithem based on pogo sort (RNG movement until goal is found or life-span is expended),
	 * Algorithem is not guarnteed to find goal thanks to RNG coupled with lifetime. (very low chance)
	 * If lifetime is removed (or expanded) probability to find goal is increased approching 1.
	 * With enough time every node will be visited.
	 */
	for (int i = 0; i < 1000; i++)
	{
		bool lCheck = true;

		while(lCheck == 1)
		{
			int lDirection = rand()%4; // Pick random number between 0 - 3;
			switch (lDirection)
			{
			case 0:
				if (aAgent.move(NORTH))
				{
					lMovementList.push_back(NORTH);
					lCheck = false;
				}
				break;
			case 1:
				if (aAgent.move(WEST))
				{
					lMovementList.push_back(WEST);
					lCheck = false;
				}
				break;
			case 2:
				if (aAgent.move(SOUTH))
				{
					lMovementList.push_back(SOUTH);
					lCheck = false;
				}
				break;
			case 3:
				if (aAgent.move(EAST))
				{
					lMovementList.push_back(EAST);
					lCheck = false;
				}
				break;
			}
		}

		if (aAgent.atGoal())
			return lMovementList;
	}
	return lMovementList;
}

/**
 * Greedy best-first
 * Use only the cost to reach the goal from the current node to evaluate the node.
 */
std::vector<Cardinal> GBFS(Agent & aAgent, Grid & aGrid)
{
	// Vectors for open and closed nodes.
	// Open list by default gets agent starting location added.
	std::vector<Coordinate> lOpen; std::vector<Coordinate> lClosed;
	lOpen.push_back(aGrid.getStartingLocation());

	// Map to contain best path
	Grid lCameFrom(CardinalStrings[NONE], aGrid.getWidth(), aGrid.getHeight());

	// Map containing cost to get from start to goal via node.
	// Cost of moving from start to goal is purly heuristic.
	std::vector<Grid> lFScores;
	for (int i = 0; i < aGrid.getGoalCount(); i++)
	{
		lFScores.push_back(Grid(std::to_string(std::numeric_limits<int>::max()), aGrid.getWidth(), aGrid.getHeight()));
		lFScores[i].setCellValue(std::to_string(HeuristicCostEstimate(aGrid.getCell(aGrid.getStartingLocation().getX(), aGrid.getStartingLocation().getY()), aGrid.getCell(aGrid.getGoalLocation(i).getX(), aGrid.getGoalLocation(i).getY()))), aGrid.getStartingLocation().getX(), aGrid.getStartingLocation().getY());
	}

	// While open list is not empty...
	while (!lOpen.empty())
	{
		// Get next from open list with lowest FScore
		Grid::Cell * lCurrent = &aGrid.getCell(lOpen[0]);
		Coordinate lCurrentCoordinate;
		int lCurrentFValue = std::numeric_limits<int>::max();

		for (int i = 0; i < lOpen.size(); i++)
		{
			for (int j = 0; j < lFScores.size(); j++)
			{
				if (std::atoi(lFScores[j].getCell(lOpen[i]).getValue().c_str()) < lCurrentFValue)
				{
					lCurrent = &aGrid.getCell(lOpen[i]);
					lCurrentCoordinate = lCurrent->getCoordinate();
					lCurrentFValue = std::atoi(lFScores[j].getCell(lCurrentCoordinate).getValue().c_str());
				}
			}
		}

		// Check is current cell is a goal.
		if (aGrid.goalAt(lCurrentCoordinate))
		{
			// Produce movement path.
			std::vector<Cardinal> lPath;
			Coordinate lCell = lCurrentCoordinate;

			print(lCameFrom);

			while (lCell != aGrid.getStartingLocation())
			{
				std::string lDir = lCameFrom.getCellValue(lCell.getX(), lCell.getY());
				if (lDir == "NORTH")
				{
					lPath.push_back(SOUTH);
					lCell = lCameFrom.getCell(lCell).getNorth().getCoordinate();
				}
				else if (lDir == "WEST")
				{
					lPath.push_back(EAST);
					lCell = lCameFrom.getCell(lCell).getWest().getCoordinate();
				}
				else if (lDir == "SOUTH")
				{
					lPath.push_back(NORTH);
					lCell = lCameFrom.getCell(lCell).getSouth().getCoordinate();
				}
				else if (lDir == "EAST")
				{
					lPath.push_back(WEST);
					lCell = lCameFrom.getCell(lCell).getEast().getCoordinate();
				}
				else
				{
					break;
				}
			}

			std::vector<Cardinal> lPathResult;
			for (int i = lPath.size() - 1; i >= 0; i--)
				lPathResult.push_back(lPath[i]);

			// Move agent to goal.
			for (int i = 0; i < lPathResult.size(); i++)
				aAgent.move(lPathResult[i]);

			return lPathResult;
		}

		// Move current from open to closed list.
		std::vector<Coordinate> lTemp;
		for (int i = 0; i < lOpen.size(); i++)
			if (lOpen[i] == lCurrentCoordinate)
				lClosed.push_back(lOpen[i]);
			else
				lTemp.push_back(lOpen[i]);

		lOpen.clear();
		lOpen = lTemp;

		// Check each direction, adding node to open list if not in open or closed lists
		for (int i = 0; i < 4; i++)
		{
			Cardinal lCameFromDirection = NONE;
			Coordinate lDirection;

			switch (i)
			{
			case 0:
				if (lCurrent->hasNorth())
					if (lCurrent->getNorth().getValue() != "WAL")
					{
						lCameFromDirection = SOUTH;
						lDirection = lCurrent->getNorth().getCoordinate();
						break;
					}
				continue;
			case 1:
				if (lCurrent->hasWest())
					if (lCurrent->getWest().getValue() != "WAL")
					{
						lCameFromDirection = EAST;
						lDirection = lCurrent->getWest().getCoordinate();
						break;
					}
				continue;
			case 2:
				if (lCurrent->hasSouth())
					if (lCurrent->getSouth().getValue() != "WAL")
					{
						lCameFromDirection = NORTH;
						lDirection = lCurrent->getSouth().getCoordinate();
						break;
					}
				continue;
			case 3:
				if (lCurrent->hasEast())
					if (lCurrent->getEast().getValue() != "WAL")
					{
						lCameFromDirection = WEST;
						lDirection = lCurrent->getEast().getCoordinate();
						break;
					}
				continue;
			}

			// if node is closed, continue to next node
			bool lInClosed = false;
			for (int j = 0; j < lClosed.size(); j++)
				if (lClosed[j] == lDirection)
					lInClosed = true;
			if (lInClosed)
				continue;

			// if node is not open, add to open
			bool lInOpen = false;
			for (int j = 0; j < lOpen.size(); j++)
			{
				if (lOpen[j] == lDirection)
				{
					lInOpen = true;
					break;
				}
			}

			if (!lInOpen)
				lOpen.push_back(lDirection);

			// Calculate FScore.
			lCameFrom.setCellValue(CardinalStrings[lCameFromDirection], lDirection.getX(), lDirection.getY());
			for (int j = 0; j < lFScores.size(); j++)
			{
				int lFTempScore = HeuristicCostEstimate(aGrid.getCell(lDirection), aGrid.getCell(aGrid.getGoalLocation(j).getX(), aGrid.getGoalLocation(j).getY()));
				lFScores[j].setCellValue(std::to_string(lFTempScore), lDirection.getX(), lDirection.getY());
			}
		}
	}

	// No path found, return empty list.
	return std::vector<Cardinal>();
}

/**
 * A-Star Search Algorithem.
 * Use both the cost to reach the goal from the current node and the cost to reach this node to evaluate the node.
 */
std::vector<Cardinal> AS(Agent & aAgent, Grid & aGrid)
{
	// Vectors for open and closed nodes.
	// Open list by default gets agent starting location added.
	std::vector<Coordinate> lOpen; std::vector<Coordinate> lClosed;
	lOpen.push_back(aGrid.getStartingLocation());

	// Map to contain best path
	Grid lCameFrom(CardinalStrings[NONE], aGrid.getWidth(), aGrid.getHeight());

	// Map to contain cost to get to node from start.
	// Cost of moving from start -> start is 0;
	Coordinate lStart = aGrid.getStartingLocation();
	Grid lGScore(std::to_string(std::numeric_limits<int>::max()), aGrid.getWidth(), aGrid.getHeight());
	lGScore.setCellValue(std::to_string(0), lStart.getX(), lStart.getY());

	// Map containing cost to get from start to goal via node.
	// Cost of moving from start to goal is purly heuristic.
	std::vector<Grid> lFScores;
	for (int i = 0; i < aGrid.getGoalCount(); i++)
	{
		lFScores.push_back(Grid(std::to_string(std::numeric_limits<int>::max()), aGrid.getWidth(), aGrid.getHeight()));
		lFScores[i].setCellValue(std::to_string(HeuristicCostEstimate(aGrid.getCell(aGrid.getStartingLocation().getX(), aGrid.getStartingLocation().getY()), aGrid.getCell(aGrid.getGoalLocation(i).getX(), aGrid.getGoalLocation(i).getY()))), aGrid.getStartingLocation().getX(), aGrid.getStartingLocation().getY());
	}

	// While open list is not empty...
	while (!lOpen.empty())
	{
		// Get next from open list with lowest FScore
		Grid::Cell * lCurrent = &aGrid.getCell(lOpen[0]);
		Coordinate lCurrentCoordinate;
		int lCurrentFValue = std::numeric_limits<int>::max();

		for (int i = 0; i < lOpen.size(); i++)
		{
			for (int j = 0; j < lFScores.size(); j++)
			{
				if (std::atoi(lFScores[j].getCell(lOpen[i]).getValue().c_str()) < lCurrentFValue)
				{
					lCurrent = &aGrid.getCell(lOpen[i]);
					lCurrentCoordinate = lCurrent->getCoordinate();
					lCurrentFValue = std::atoi(lFScores[j].getCell(lCurrentCoordinate).getValue().c_str());
				}
			}
		}

		// Check is current cell is a goal.
		if (aGrid.goalAt(lCurrentCoordinate))
		{
			// Produce movement path.
			std::vector<Cardinal> lPath;
			Coordinate lCell = lCurrentCoordinate;

			print(lCameFrom);

			while (lCell != aGrid.getStartingLocation())
			{
				std::string lDir = lCameFrom.getCellValue(lCell.getX(), lCell.getY());
				if (lDir == "NORTH")
				{
					lPath.push_back(SOUTH);
					lCell = lCameFrom.getCell(lCell).getNorth().getCoordinate();
				}
				else if (lDir == "WEST")
				{
					lPath.push_back(EAST);
					lCell = lCameFrom.getCell(lCell).getWest().getCoordinate();
				}
				else if (lDir == "SOUTH")
				{
					lPath.push_back(NORTH);
					lCell = lCameFrom.getCell(lCell).getSouth().getCoordinate();
				}
				else if (lDir == "EAST")
				{
					lPath.push_back(WEST);
					lCell = lCameFrom.getCell(lCell).getEast().getCoordinate();
				}
				else
				{
					break;
				}
			}

			std::vector<Cardinal> lPathResult;
			for (int i = lPath.size() - 1; i >= 0; i--)
				lPathResult.push_back(lPath[i]);

			// Move agent to goal.
			for (int i = 0; i < lPathResult.size(); i++)
				aAgent.move(lPathResult[i]);

			return lPathResult;
		}

		// Move current from open to closed list.
		std::vector<Coordinate> lTemp;
		for (int i = 0; i < lOpen.size(); i++)
			if (lOpen[i] == lCurrentCoordinate)
				lClosed.push_back(lOpen[i]);
			else
				lTemp.push_back(lOpen[i]);

		lOpen.clear();
		lOpen = lTemp;

		// Check each direction, adding node to open list if not in open or closed lists
		for (int i = 0; i < 4; i++)
		{
			Cardinal lCameFromDirection = NONE;
			Coordinate lDirection;

			switch (i)
			{
			case 0:
				if (lCurrent->hasNorth())
					if (lCurrent->getNorth().getValue() != "WAL")
					{
						lCameFromDirection = SOUTH;
						lDirection = lCurrent->getNorth().getCoordinate();
						break;
					}
				continue;
			case 1:
				if (lCurrent->hasWest())
					if (lCurrent->getWest().getValue() != "WAL")
					{
						lCameFromDirection = EAST;
						lDirection = lCurrent->getWest().getCoordinate();
						break;
					}
				continue;
			case 2:
				if (lCurrent->hasSouth())
					if (lCurrent->getSouth().getValue() != "WAL")
					{
						lCameFromDirection = NORTH;
						lDirection = lCurrent->getSouth().getCoordinate();
						break;
					}
				continue;
			case 3:
				if (lCurrent->hasEast())
					if (lCurrent->getEast().getValue() != "WAL")
					{
						lCameFromDirection = WEST;
						lDirection = lCurrent->getEast().getCoordinate();
						break;
					}
				continue;
			}

			// if node is closed, continue to next node
			bool lInClosed = false;
			for (int j = 0; j < lClosed.size(); j++)
				if (lClosed[j] == lDirection)
					lInClosed = true;
			if (lInClosed)
				continue;

			// if node is not open, add to open
			bool lInOpen = false;
			for (int j = 0; j < lOpen.size(); j++)
			{
				if (lOpen[j] == lDirection)
				{
					lInOpen = true;
					break;
				}
			}

			if (!lInOpen)
				lOpen.push_back(lDirection);

			// Calculate GScore and FScores
			int lGTempScore = std::atoi(lGScore.getCellValue(lCurrentCoordinate.getX(), lCurrentCoordinate.getY()).c_str()) + CalculateDistance(lCurrentCoordinate, lDirection);

			// If not a better path to node, continue to next node
			if (lGTempScore >= std::atoi(lGScore.getCellValue(lDirection.getX(), lDirection.getY()).c_str()))
				continue;

			// If better path then record it.
			lCameFrom.setCellValue(CardinalStrings[lCameFromDirection], lDirection.getX(), lDirection.getY());
			lGScore.setCellValue(std::to_string(lGTempScore), lDirection.getX(), lDirection.getY());
			for (int j = 0; j < lFScores.size(); j++)
			{
				int lFTempScore = lGTempScore + HeuristicCostEstimate(aGrid.getCell(lDirection), aGrid.getCell(aGrid.getGoalLocation(j).getX(), aGrid.getGoalLocation(j).getY()));
				lFScores[j].setCellValue(std::to_string(lFTempScore), lDirection.getX(), lDirection.getY());
			}
		}
	}

	// No path found, return empty list.
	return std::vector<Cardinal>();
}

std::vector<Cardinal> CUS2(Agent & aAgent, Grid & aGrid)
{
	bool lGoalFound = false;

	std::vector<Coordinate> lVisited;
	lVisited.push_back(aAgent.getCoordinate()); // Add inital location to visited list.

	std::vector<Cardinal> lPathToGoal;

	CUS2Thread(aGrid, aAgent.getCoordinate(), lVisited, std::vector<Cardinal>(), lGoalFound, lPathToGoal);

	// if goal has been found move agent to goal.
	if(lGoalFound)
		for (int i = 0; i < lPathToGoal.size(); i++)
			aAgent.move(lPathToGoal[i]);

	return lPathToGoal;
}

void CUS2Thread(Grid & aGrid, Coordinate aThisCell, std::vector<Coordinate>& aVisited, std::vector<Cardinal> aPathToHere, bool & aGoalFound, std::vector<Cardinal> & aPathToGoal)
{
	// Check if this cell is at goal
	if (aGrid.goalAt(aThisCell))
	{
		bool lFound = true;
		aGoalFound = lFound;
		aPathToGoal = aPathToHere;
		return;
	}

	// Get the details for each valid cell for aditional threads to assess.
	std::vector<Coordinate> lValidCells;
	std::vector<std::vector<Cardinal>> lPathToCells;

	for (int i = 0; i < 4; i++)
	{
		Coordinate lDirection;
		Cardinal lMovement;
		bool lVisited = false;
		switch (i)
		{
		case NORTH:
			if (aGrid.getCell(aThisCell).hasNorth())
				lDirection = aGrid.getCell(aThisCell).getNorth().getCoordinate();
			else
				continue;
			lMovement = NORTH;
			break;
		case WEST:
			if(aGrid.getCell(aThisCell).hasWest())
				lDirection = aGrid.getCell(aThisCell).getWest().getCoordinate();
			else 
				continue;
			lMovement = WEST;
			break;
		case SOUTH:
			if (aGrid.getCell(aThisCell).hasSouth())
				lDirection = aGrid.getCell(aThisCell).getSouth().getCoordinate();
			else
				continue;
			lMovement = SOUTH;
			break;
		case EAST:
			if (aGrid.getCell(aThisCell).hasEast())
				lDirection = aGrid.getCell(aThisCell).getEast().getCoordinate();
			else
				continue;
			lMovement = EAST;
			break;
		}
		// Check if next node valid to traverse to.
		for (int j = 0; j < aVisited.size(); j++)
			if (lDirection == aVisited[j])
				lVisited = true;

		if (lVisited)
			break;

		// Crucial data section. locked with mutex lock
		std::vector<Cardinal> lPathToDirection = aPathToHere;
		lPathToDirection.push_back(lMovement);
		lPathToCells.push_back(lPathToDirection);
		lValidCells.push_back(lDirection);

		gMtx.lock();
		aVisited.push_back(lDirection);
		gMtx.unlock();
	}
	CUS2NewThreads(aGrid, lValidCells, aVisited, lPathToCells, aGoalFound, aPathToGoal);
}

void CUS2NewThreads(Grid & aGrid, std::vector<Coordinate> aCellsToCheck, std::vector<Coordinate>& aVisited, std::vector<std::vector<Cardinal>> aPathToCells, bool & aGoalFound, std::vector<Cardinal>& aPathToGoal)
{
	/*
	// Setup Threads
	std::vector<std::thread> lThreads;
	for (int i = 0; i < aCellsToCheck.size(); i++)
		lThreads.push_back(new std::thread(CUS2Thread, aGrid, aCellsToCheck[i], aVisited, aPathToCells[i], aGoalFound, aPathToGoal));
	// Execute Threds
	for (auto& lTh : lThreads)
		lTh.join();
		*/
	for (int i = 0; i < aCellsToCheck.size(); i++)
		CUS2Thread(aGrid, aCellsToCheck[i], aVisited, aPathToCells[i], aGoalFound, aPathToGoal);
}

int HeuristicCostEstimate(Grid::Cell & aFrom, Grid::Cell & aTo)
{
	// if either node is a wall return infinity
	if (aFrom.getValue() == "WAL" || aTo.getValue() == "WAL")
		return std::numeric_limits<int>::max();

	//Calulate Distance Heuristic
	int lXDist = std::abs(aFrom.getX() - aTo.getX());
	int lYDist = std::abs(aFrom.getY() - aTo.getY());
	int lManhatten = lXDist + lYDist;
	int lEuclidian = std::sqrt((lXDist * lXDist) + (lYDist * lYDist));
	return lManhatten + lEuclidian;
}

int CalculateDistance(Coordinate & aFrom, Coordinate & aTo)
{
	int lXDist = std::abs(aFrom.getX() - aTo.getX());
	int lYDist = std::abs(aFrom.getY() - aTo.getY());
	int lEuclidian = std::sqrt((lXDist * lXDist) + (lYDist * lYDist));
	return lEuclidian;
}
