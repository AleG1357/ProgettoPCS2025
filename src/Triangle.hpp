#pragma once

#include <limits>
#include <utility>

#include "Polyhedron.hpp"


// Function which checks if the new vertex already exists
unsigned int addVertexIfMissing(Polyhedron& P, const Vector3d coords_V);

// Function which checks if the new edge already exists 
unsigned int addEdgeIfMissing(Polyhedron& P, unsigned int id1, unsigned int id2,
							  unsigned int id_e = numeric_limits<unsigned int>::max());

// Functions for Class I and Class II triangulation of a polyhedron
Polyhedron TriangleClassI(const Polyhedron& P_old, const unsigned int& val);
Polyhedron TriangleClassII(const Polyhedron& P_old, const unsigned int& val);