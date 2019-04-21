#pragma once

/**
 * Printers, defines printing functions for program output, all printers work by provideing referance to object/s been printed.
 * @version v1.0.1
 * @author	James Galovic - 6819710
 * @date	17-04-2019
 */

#include "Grid.h"
#include "Agent.h"

void print(Grid & aGrid);
void print(Grid & aGrid, Agent & aAgent);
