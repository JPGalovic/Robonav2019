# Robonav2019
James P. Galovic - 6819710
Solution to Robonav Problem for COS30019

## Introduction
This repository contains the solution developed for COS30019 - Intro to artificial intelegance, Assignment 1, Option B: Robot Navigation.

## Search Algorithems
This program implements the following Search Algorithms:

* Depth-first Search: Select one option, try it, go back when ther are no more options.
* Breadth-first Search: Expand all options one level at a time.
* Greedy Best-first: Use only the cost to reach the goal from the current node to evaluate the node.
* A* ("A Star"): Use both the cost to reach the goal from the current node and the cost to reach this node to evaluate the node.
* Custom Search Stratagy 1: An uninformed method to find a path to reach the goal.
* Custom Search Stratagy 2: An informed method to find a shortest path (with least moves) to reach the goal.

## Implementaion
The implementation of the solution included not only development of the search algorithems but also the enviroment and loading processes for the AI agent to operate within.

### Graph
The primary enviroment is stored in an object called Graph, the term graph was chosen over map due to the design of the enviroment been based on a grid of cells connected in four directions, North, West, South and East.

The graph, rather than been a 2D array, is built simmilar to a Doubly Linked List, where the graph is refernced from an origin node, which gets doubly linked to nodes that may exist in each cardinal direction.

This implementation can simply adapted into a traditional map (node with n connections) through simple alterations to the class.

The graph object contains two main Contstruction methods, one where FileIO is used to generate the enviroment, and another where a blank graph of set dimentions is generated with a given default value.
```c++
Grid(const std::string aFileName);
Grid(const std::string aValue, int aWidth, int aHeight);
```

#### FileIO
FileIO is used to generate the enviroment, in this case the File Path is all that is provided to the constructer.
The file needs to be formatted correctly for reading, and consists of N lines, at least 3.
Lines 0 - 3 contain crucial information for producing the enviroment. Descriptions and formatting of the file is as follows:
```c++
[w,h]               // Width and height of the enviroment
(x,y)               // Starting location of the agent
(x1,y1) | (x2,y2)   // Goal states for the agent - coordinates are seperated by the character |
(x,y,w,h)           // Refernce to a rectangular block of walls the agent cannot pass through, the walls leftmost top corner occupies cell(x,y) and is w cells wide, and h cess tall
...                 // N number of walls may be definied
```

### Agent
The Agent (Robot) contains the required functionality to manover arround the Grid, keep track of its position and determine which paths it is able to take, and weather it is at a goal state.

While the agent contains a pointer the the Grid, it is not used internally for any path-finding, infact all search algorithms are external to the Agent Object.

In adition to the Agent Class, the header (and thus source) of the Agent also contain descrete functions for each Search Algorithem, as well as the auxiliary functions where required.

#### Un-Informed Algorithms
Three un-informed Algorithms have been defined, common tree-search methods, Depth-first Search and Breadth-first Search, as well as a Custom un-informed algorithem based on the sorting method known as "pogo-sort"

##### Depth-first Search (DFS)
The depth-first search algorithem is built on a recursive - tree builder, requireing one overload function to fit within a stardard call format. initally the DFS function is called and a referance to the agent provided. the function generates a vector to store the coordinates of visited nodes, and calls the aux. function that process the recussive pattern of the algorithm, this works on the following psedo-code

```c++
  if(Agent.atGoal())
    return path;
    
  foreach(valid connection)
    if(!visited)
      visit();
      
  goback();
```

##### Breadth-first Search (BFS)
The breadth-first seach algorithm is designed to assess each node on a given level before moving to the next, the agent achives this by constructing a queue of nodes to search, and visits the dermined node, adding the valid nodes from that point to the back of the queue.
because the agent is only able to see the node it is currently located at, the agent needs to travel to the given node to assess it. as such the implementaion of this algorithems contains rules for paths from the origin to each node based on the algorithem.

The agent returns to the origin before proceeding to the next node in the queue, until a goal is reached, or all paths are fully explored. the algoithem is designed arround the following psedo-code

```c++
  do
  {
    foreach(valid connection)
      enqueue.(path to valid connection);
    
    move to origin node;
    move to next node in queue;
    
    if(Agent.atGoal())
      return path;
      
  } while (!queue.empty())
```

##### Custom Un-informed Search Algorithm (CUS1)
The custom un-informed search algorithm is based on the array sorting method known as pogo-sort, it is a ineffective search alogithm. the design of the algorithem uses an agent lifespan and is based on the following psedo-code

```c++
while(Agent.isAlive())
{
  Move in a random direction();
  if(Agent.atGoal())
      return path;
}
```
In this implementation the life of the agent is limited at 1000 movements.

#### Informed Algorithms
Three informed Algorithms have been defined, Greedy Best-first Search, A* (A Star) as well as a Custom informed algorithem based on the that thanks to been informed can use simaltanious breadth-first search (multithreaded with locked visited list).

##### Distance Cost and Heuristic Cost Estimate
The greedy best-first search and A* search algorithms require a auxiluary cost calulation method to work correctly. two such fuctions have been developed. calculating the cost based on the positioning of a to and from node on the grid.

```c++
int HeuristicCostEstimate(Grid::Cell & aFrom, Grid::Cell & aTo);
int CalculateDistance(Coordinate & aFrom, Coordinate & aTo);
```

Both functions use a sell known distance method known as Euclidian Distance, this is an "as the bird flys distance" it is calulated using Pythagoras' theorem, where the euclidian distance between two points squared is the differance in x squared and the differnce in y squared (a^2 + b^2 = c^2).

in adition to this another common measurement method, which is used in the Heuristic Cost Estimate, is Manhattan distance, i.e. the distance required to travel along the x and y paths.

the final heuristic cost estimate used is the sum of euclidan and manhatten distances.

##### Greedy Best-first Search (GBFS)

##### A* ("AStar") (AS)
The A* search algoithm uses both the cost to travel from the starting point to the node, as well as the cost to travel from the node to the goal to assess the node to determine the best path. the function is built on the following psudo-code

```c++
initalise a grid to store the fScores and gScores for nodes, as well as a grid to contain direction of the best node to travel to this node.
the gScore of moving from stat to start is 0
the fScore of moving from the start to goal is purely heuristic

initalise openlist containing starting node.

while(!openList.empty())
{
  get next node from open list with lowest fScore;
  
  if(node is at goal)
    construct path;
    
  move current node to closedList;
  
  foreach(validDirection)
  {
    if(validDirection isnt in closedList)
    {
      if(validDirection isnt in openList)
        add validDirection to openList;
    }
    
    Calculate GScores and FScores
    
    If better path to currentNode found record it
  }
}
```

##### Custom Informed Search Algorithem (CUS2)
The second Custom Search Algorithem is designed as a Threaded Informed Simaltanious Node search.
It uses a combination of an informed depth first search algorithem and along with threading to allow simaltaniuos path determination, without the need for the agent to move to search.

each thread will assess a single node, producing aditional threads to assess valid, un-visited nodes. the threads will follow the following psudo-code

```c++
  if(thisCell.atGoal())
    generate path;
    
  foreach(valid connection)
    if(!visited)
      start new thread to visit();
```

once the goal has been found the agent 

## Featurs/Bugs/Missing
### Notable Features
All algorithms contained within single executible, algorithm switching achived through program arguments, when launching the programs you can slect from any of the above-mentioned algorithms, simply use the correct code, DFS, BFS, GBFS, AS, CUS1 or CUS2. the program will load the enviroment from file and then execute the chosen algorithem.

While the agent moves a display of the enviroment is presented to the user so that the path may not oly bve read the path but also visualise the enviroment and how the agent manovers arround the enviroment, this is perticually useful for understanding the un-informed methods.

### Bugs & Missing Features
BUG - Backtracking Failue when executing Bredth First Search Algorithm
Agent will continute to move systematically until a goal is found, and thanks to the methods of movement been correct the agent will move correctly and (in all tested cases) navigate to a goal.
Bug is suspected to be cause by the process used for backtracking to the starting point, in conjunction with a possible error in queueing due to error in reading visited list. alternitivly, the error could exist due to the method been used to track path back to origin.
Further diagnosis is required to correctly determine and fix cause.

BUG - Threading in CUS2 algorithm.
Due to my inexperiance in C++ threading (at this time), when attempting to inintalise threads to analise the valid nodes (equivilant of froming queue in BFS algorithm).
Bug is cause of CUS2 algorithm to be ineffective.

## Conclusion
In simmilar cases to this I would use an A* algorithem, as this is a simple maze solver and the goal is not flexable nor is there a competing entity (player or otherwise). other search method may be used depending on what changes are made to the base task.

## Acknowledgements/Resorces
Both the bredth-first search and depth-first search algorithms are taught in the unit, Data Structures and Patterns, the algorithems were developed based on knowledge learned in that unit.

The a* algorithm was transposed and improved from a previous attempt of this unit.

The custom altorithems were developed as an experiment researching into effective useses of a agent lifespan (CUS1) and an experiment in threaded search algorithems (CUS2)
