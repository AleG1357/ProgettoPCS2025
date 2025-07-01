#pragma once

#include <map>
#include <algorithm>

#include "Polyhedron.hpp"


// Function which normalizes the vertices (Geometry.cpp)
void normalizeVertex(Vertex& v);


// Function which computes the barycenter of a face (Geometry.cpp)
Vertex Barycenter(const Polyhedron& P, const unsigned int& f_id);


// Functions which find the neighbors of each edge and vertex
void getEdgeNeighbors(Polyhedron& P);
void getVertexNeighbors(Polyhedron& P);


// Function which creates the dual of the polyhedron (Geometry.cpp)
Polyhedron Dual(const Polyhedron& P);