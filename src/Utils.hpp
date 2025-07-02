#pragma once

#include "Polyhedron.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;

Polyhedron buildPlatonicSolid(int p, int q);

// Function which exports the polyhedron for Paraview
void exportPolyhedron(const Polyhedron& P);

// Function which writes the output
bool writeOutput(const Polyhedron& P);