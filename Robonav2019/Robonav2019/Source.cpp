#include "Grid.h"
#include "Agent.h"
#include "Printers.h"

#include <iostream>
#include <algorithm>
#include <stdexcept>

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "Executiable parameters not set correctly, please use the following paramiter: <FILEPATH> <METHOD>" << std::endl;
		system("PAUSE");
		return 1;
	}
	
	try
	{
		std::string lFileName = argv[1];
		std::string lMethod = argv[2];

		std::transform(lMethod.begin(), lMethod.end(), lMethod.begin(), toupper);

		Grid lGrid(lFileName);
		Agent lAgent(lGrid);

		std::vector<Cardinal> lPath;

		if (lMethod == "DFS")
			lPath = DFS(lAgent);
		else if (lMethod == "BFS")
			lPath = BFS(lAgent);
		else if (lMethod == "CUS1")
			lPath = CUS1(lAgent);
		else if (lMethod == "GBFS")
			lPath = GBFS(lAgent, lGrid);
		else if (lMethod == "AS")
			lPath = AS(lAgent, lGrid);
		else if (lMethod == "CUS2")
			lPath = CUS2(lAgent, lGrid);
		else
		{
			std::cerr << "The method " << lMethod << " cannot be handled by this Agent or does not exist. please use methods: DFS, BFS, GBFS, AS, CUS1, CUS2" << std::endl;
			std::system("PAUSE");
			return 3;
		}
		
		std::system("CLS");
		std::cout << lFileName << " " << lMethod << " " << lPath.size() << std::endl;

		if (!lAgent.atGoal())
		{
			std::cout << "No solution found.";
		}
		else
		{
			for (int i = 0; i < lPath.size(); i++)
			{
				if (i > 0)
					std::cout << ", ";

				switch (lPath[i])
				{
				case NORTH:
					std::cout << "NORTH";
					break;
				case WEST:
					std::cout << "WEST";
					break;
				case SOUTH:
					std::cout << "SOUTH";
					break;
				case EAST:
					std::cout << "EAST";
					break;
				default:
					break;
				}
			}
		}
		std::cout << std::endl;
		std::system("PAUSE");
	}
	catch (const std::runtime_error & e)
	{
		std::cerr << e.what() << std::endl;
		std::system("PAUSE");
		return 2;
	}

	return 0;
}