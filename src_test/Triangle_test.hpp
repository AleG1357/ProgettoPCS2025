// #pragma once

// #include <gtest/gtest.h>

// #include "Triangle.hpp"


// using namespace std;



// // Addition TEST

// TEST(AdditionTest, VertexAddition)
// {
// 	// Create a polyhedron object
// 	Polyhedron P;
	
// 	// Add a vertex
// 	Vector3d v0_coords = {1,2,3};
// 	unsigned int id0 = addVertexIfMissing(P, v0_coords);
// 	ASSERT_EQ(id0, 0);
// 	ASSERT_EQ(P.numVertices(), 1);
	
// 	// Add a near duplicate
// 	Vector3d v1_coords = {1 + numeric_limits<double>::epsilon(),
// 						  2 + numeric_limits<double>::epsilon(),
// 						  3 + numeric_limits<double>::epsilon()};
// 	unsigned int id1 = addVertexIfMissing(P, v1_coords);
// 	ASSERT_EQ(id1, 0);
// 	ASSERT_EQ(P.numVertices(), 1);
	
// 	// Add a different vertex
// 	Vector3d v2_coords = {4,5,6};
// 	unsigned int id2 = addVertexIfMissing(P, v2_coords);
// 	ASSERT_EQ(id2, 1);
// 	ASSERT_EQ(P.numVertices(), 2);
// }


// TEST(AdditionTest, EdgeAddition)
// {
// 	// Create a polyhedron with 3 vertices
// 	Polyhedron P;
// 	P.vertices = {{0, {0,0,0}, {0,0,0}, {0,0,0}},
// 				  {1, {1,0,0}, {0,0,0}, {0,0,0}},
// 				  {2, {0,1,0}, {0,0,0}, {0,0,0}}};

// 	// Add a valid edge
// 	unsigned int id0 = addEdgeIfMissing(P, 0, 1);
// 	ASSERT_EQ(id0, 0);
// 	ASSERT_EQ(P.numEdges(), 1);
	
// 	// Add a duplicate (different order)
// 	unsigned int id1 = addEdgeIfMissing(P, 1, 0);
// 	ASSERT_EQ(id1, 0);
// 	ASSERT_EQ(P.numEdges(), 1);
	
// 	// Add invalid edges
// 	unsigned int id2 = addEdgeIfMissing(P, 1, 1);
// 	unsigned int id3 = addEdgeIfMissing(P, 1, 4);
// 	ASSERT_EQ(id2, numeric_limits<unsigned int>::max());
// 	ASSERT_EQ(id3, numeric_limits<unsigned int>::max());
// 	ASSERT_EQ(P.numEdges(), 1);
	
// 	// Add a different edge
// 	unsigned int id4 = addEdgeIfMissing(P, 1, 2);
// 	ASSERT_EQ(id4, 1);
// 	ASSERT_EQ(P.numEdges(), 2);
	
// 	// Add a different edge with a requested ID
// 	unsigned int id5 = addEdgeIfMissing(P, 2, 0, 5);
// 	ASSERT_EQ(id5, 5);
// 	ASSERT_EQ(P.numEdges(), 3);
// }


// // Triangle TEST

// TEST(TriangleTest, ClassI)
// {
// 	// Create a tetrahedron
// 	Polyhedron P_old = Tetrahedron();

// 	// Perform Class I triangulation with val = 2
// 	Polyhedron P = TriangleClassI(P_old, 2);

// 	// We expect 10 vertices, 24 edges and 16 faces
// 	ASSERT_EQ(P.numVertices(), 10);
// 	ASSERT_EQ(P.numEdges(), 24);
// 	ASSERT_EQ(P.numFaces(), 16);

// 	// Check that all faces have 3 vertices and 3 edges
// 	for (const auto& f : P.faces)
// 	{
// 		ASSERT_EQ(f.idVertices.size(), 3);
// 		ASSERT_EQ(f.idEdges.size(), 3);
// 	}

// 	// Check that there are no invalid edges
// 	for (const auto& e : P.edges)
// 	{
// 		ASSERT_NE(e.origin, e.end);            // NE = Not Equal
// 		ASSERT_LT(e.origin, P.numVertices());  // LT = Less Than
// 		ASSERT_LT(e.end, P.numVertices());
// 	}
	
// 	ASSERT_TRUE(P.checkFaces());
// }


// TEST(TriangleTest, ClassII)
// {
// 	// Create a tetrahedron
// 	Polyhedron P_old = Tetrahedron();

// 	// Perform Class II triangulation with val = 2
// 	Polyhedron P = TriangleClassII(P_old, 2);

// 	// We expect 26 vertices, 72 edges and 48 faces
// 	ASSERT_EQ(P.numVertices(), 26);
// 	ASSERT_EQ(P.numEdges(), 72);
// 	ASSERT_EQ(P.numFaces(), 48);

// 	// Check that all faces have 3 vertices and 3 edges
// 	for (const auto& f : P.faces)
// 	{
// 		ASSERT_EQ(f.idVertices.size(), 3);
// 		ASSERT_EQ(f.idEdges.size(), 3);
// 	}

// 	// Check that there are no invalid edges
// 	for (const auto& e : P.edges)
// 	{
// 		ASSERT_NE(e.origin, e.end);            // NE = Not Equal
// 		ASSERT_LT(e.origin, P.numVertices());  // LT = Less Than
// 		ASSERT_LT(e.end, P.numVertices());
// 	}

// 	ASSERT_TRUE(P.checkFaces());
// }