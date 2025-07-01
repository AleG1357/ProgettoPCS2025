#pragma once

#include <queue>
#include <list>

#include "Polyhedron.hpp"


// Graph struct containing the adjacency matrix
struct Graph
{
    vector<list<unsigned int>> adjacencyList;
};


// Function which creates the graph 
Graph createGraph(const Polyhedron& pol);

// Function which creates the weights matrix
MatrixXd createWeights(const Polyhedron& pol);

// Function which finds the shortest path using Diskstra's algorithm
vector<unsigned int> Dijkstra(const Graph& graph,
                                const unsigned int& id_path_start,
                                const unsigned int& id_path_end,
                                const MatrixXd& weights);

// Function which prints the path in the terminal
void printPath(Polyhedron& pol, const vector<unsigned int> path);

// Function which draws the path on the polyhedron
void drawPath(Polyhedron& pol, const vector<unsigned int> path);