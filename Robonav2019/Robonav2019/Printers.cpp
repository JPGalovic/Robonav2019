#include "Printers.h"

#include <iostream>
#include <Windows.h>
#include <thread>

void print(Grid & aGrid)
{
	std::system("CLS");
	std::cout << "Grid size: [" << aGrid.getWidth() << ", " << aGrid.getHeight() << "]." << std::endl;
	HANDLE lConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int j = 0; j < aGrid.getHeight(); j++)
	{
		for (int i = 0; i < aGrid.getWidth(); i++)
		{
			if(aGrid.getCellValue(i,j) == "CLR")
				SetConsoleTextAttribute(lConsole, 10);
			if (aGrid.getCellValue(i, j) == "WAL")
				SetConsoleTextAttribute(lConsole, 12);
			std::cout << " " << aGrid.getCell(i, j).getValue();
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	SetConsoleTextAttribute(lConsole, 15);
}

void print(Grid & aGrid, Agent & aAgent)
{
	std::system("CLS");
	std::cout << "Grid size: [" << aGrid.getWidth() << ", " << aGrid.getHeight() << "]." << std::endl;
	HANDLE lConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int j = 0; j < aGrid.getHeight(); j++)
	{
		for (int i = 0; i < aGrid.getWidth(); i++)
		{
			if (aAgent.isAt(i, j))
			{
				SetConsoleTextAttribute(lConsole, 11);
				std::cout << " " << "AGT";
				continue;
			}
			if (aGrid.goalAt(Coordinate(i, j)))
			{
				SetConsoleTextAttribute(lConsole, 14);
				std::cout << " " << "GOL";
				continue;
			}
			if (aGrid.getCellValue(i, j) == "CLR")
				SetConsoleTextAttribute(lConsole, 10);
			if (aGrid.getCellValue(i, j) == "WAL")
				SetConsoleTextAttribute(lConsole, 12);
			std::cout << " " << aGrid.getCell(i, j).getValue();
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	SetConsoleTextAttribute(lConsole, 15);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
