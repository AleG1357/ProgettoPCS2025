#pragma once

#include "Polyhedron.hpp"


// Proietta un vertice sulla sfera unitaria (Geometry.cpp)
void projectOnUnitSphere(Vertex& v);

// Calcola il centroide di una faccia (Geometry.cpp)
Vertex faceCentroid(const Polyhedron& poly, unsigned int faceId);

// Assegna le adiacenze tra edge e facce
void assignEdgeAdjacencies(Polyhedron& poly);
// Assegna le adiacenze tra vertici, edge e facce
void assignVertexAdjacencies(Polyhedron& poly);

// Costruisce il poliedro duale
Polyhedron makeDualPolyhedron(const Polyhedron& poly);