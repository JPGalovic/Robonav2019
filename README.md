# Robonav2019
James P. Galovic - 6819710
Solution to Robonav Problem for COS30019

## Introduction
This repository contains the solution developed for COS30019 - Intro to artificial intelegance, Assignment 1, Option B: Robot Navigation.

## Search Algorithems
This program implements the following Search Algorithms:

Depth-first Search: Select one option, try it, go back when ther are no more options.
Breadth-first Search: Expand all options one level at a time.

Greedy Best-first: Use only the cost to reach the goal from the current node to evaluate the node.
A* ("A Star"): Use both the cost to reach the goal from the current node and the cost to reach this node to evaluate the node.

Custom Search Stratagy 1: An uninformed method to find a path to reach the goal.
Custom Search Stratagy 2: An informed method to find a shortest path (with least moves) to reach the goal.

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

##### Custom Un-informed Search Algorithm 1 (CUS1)
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

## Featurs/Bugs/Missing

## Research

## Conclusion

## References


