#pragma once

#include <queue>
#include <list>

#include "Polyhedron.hpp"


// Struttura grafo: lista di adiacenza (più efficiente per BFS/Dijkstra)
struct Graph {
    std::vector<std::vector<unsigned int>> adjacencyList;
};


// Costruzione grafo e pesi
Graph buildAdjacencyGraph(const Polyhedron& poly);
MatrixXd buildEdgeWeightMatrix(const Polyhedron& poly);

// Algoritmi di cammino minimo
std::vector<unsigned int> dijkstraPath(const Graph& g, unsigned int start, unsigned int end, const MatrixXd& W);
std::vector<unsigned int> bfsPath(const Graph& g, unsigned int start, unsigned int end);
std::vector<unsigned int> findShortestPath(const Graph& g, unsigned int start, unsigned int end, bool isUniformEdgeLength, const MatrixXd& W);

// Output e visualizzazione
void printPath(const Polyhedron& poly, const std::vector<unsigned int>& path);
void highlightPath(Polyhedron& poly, const std::vector<unsigned int>& path);