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

## Featurs/Bugs/Missing

## Research

## Conclusion

## References


