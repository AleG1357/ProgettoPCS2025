#pragma once

#include <limits>
#include <utility>

#include "Polyhedron.hpp"


// Funzione che aggiunge un vertice solo se non già presente
unsigned int addVertexIfMissing(Polyhedron& poly, const Vector3d& coords);

// Funzione che aggiunge un edge solo se non già presente
unsigned int addEdgeIfMissing(Polyhedron& poly, unsigned int id1, unsigned int id2, unsigned int forced_id);
// Overload per chiamate senza forced_id (comportamento di default)
inline unsigned int addEdgeIfMissing(Polyhedron& poly, unsigned int id1, unsigned int id2) {
	return addEdgeIfMissing(poly, id1, id2, numeric_limits<unsigned int>::max());
}

// Triangolazione Class I e II di un poliedro
Polyhedron TriangleClassI(const Polyhedron& P_old, const unsigned int& val);
Polyhedron TriangleClassII(const Polyhedron& P_old, const unsigned int& val);