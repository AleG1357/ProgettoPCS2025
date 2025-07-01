#pragma once

#include <fstream>
#include <sstream>
#include <numbers>

#include "Polyhedron.hpp"
#include "UCDUtilities.hpp"


using namespace std;


// Function which checks if values of polyhedron input are correct
bool checkPolyhedronInput(int argc, char* argv[],
				 		unsigned int &p, unsigned int &q,
				 		unsigned int &b, unsigned int &c,
				 		unsigned int &val, unsigned int &flag);

// Function which checks if values of graph input are correct
bool checkGraphInput(char* argv[],
					unsigned int& id_path_start, 
					unsigned int& id_path_end, 
					const Polyhedron& P);

// // Functions that create tetrahedron, octahedron and icosahedron
// Polyhedron Tetrahedron();
// Polyhedron Octahedron();
// Polyhedron Icosahedron();

Polyhedron buildPlatonicSolid(int p, int q);

// Function which exports the polyhedron for Paraview
void exportPolyhedron(const Polyhedron& P);

// Function which writes the output
bool writeOutput(const Polyhedron& P);