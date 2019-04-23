#pragma once

/**
 * Agent, used to define AI player for game.
 * @version	1.1.0
 * @author	James Galovic - 6819710
 * @date	17-04-2019
 */

#include "Grid.h"
#include "Cardinal.h"

#include <vector>

class Agent
{
private:
	Coordinate fLocation;
	Grid * fGrid; // Grid referance for 

public:
	Agent(Grid & aGrid);

	// Agent positioning & movement.
	void resetPosition();

	bool move(Cardinal aDirection);
	bool moveTo(Grid::Cell & aCell);

	Coordinate getCoordinate();
	bool validMovement(Cardinal aDirection);

	// Agent positioning checking.
	bool atGoal();
	bool isAt(int aX, int aY);
};

// Search Algorithems
// Un-informed
std::vector<Cardinal> DFS(Agent & aAgent);
std::vector<Cardinal> DFS(Agent & aAgent, std::vector<Coordinate> & aVisited, Cardinal aDirection);
std::vector<Cardinal> BFS(Agent & aAgent);
std::vector<Cardinal> CUS1(Agent & aAgent);
// Informed
std::vector<Cardinal> GBFS(Agent & aAgent, Grid & aGrid);
std::vector<Cardinal> AS(Agent & aAgent, Grid & aGrid);
std::vector<Cardinal> CUS2(Agent & aAgent, Grid & aGrid);
void CUS2Thread(Grid & aGrid, Coordinate aThisCell, std::vector<Coordinate>& aVisited, std::vector<Cardinal> aPathToHere, bool & aGoalFound, std::vector<Cardinal> & aPathToGoal);
void CUS2NewThreads(Grid & aGrid, std::vector<Coordinate> aCellsToCheck, std::vector<Coordinate> & aVisited, std::vector<std::vector<Cardinal>> aPathCells, bool & aGoalFound, std::vector<Cardinal> & aPathToGoal);

// Aux Functions
int HeuristicCostEstimate(Grid::Cell & aFrom, Grid::Cell & aTo);
int CalculateDistance(Coordinate & aFrom, Coordinate & aTo);