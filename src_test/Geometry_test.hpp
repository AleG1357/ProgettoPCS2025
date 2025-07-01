// #pragma once

// #include <gtest/gtest.h>

// #include "Geometry.hpp"


// using namespace std;



// // Geometry TEST

// TEST(GeometryTest, VertexNormalization)
// {
// 	// Create a vertex 
// 	Vertex v = {0, {4.3, 5.0, 6.8}, {0,0,0}, {0,0,0}};

// 	// Normalize the vertex
// 	normalizeVertex(v);

// 	// Check that the function worked correctly
// 	double length = v.coords.norm();
// 	ASSERT_NEAR(length, 1.0, numeric_limits<double>::epsilon());
// }


// TEST(GeometryTest, BarycenterComputation)
// {
// 	// Create a polyhedron object
// 	Polyhedron P;

// 	// Create vertices
// 	P.vertices = 
// 	{
// 		{0, {0, 0, 0}, {0,0,0}, {0,0,0}},
// 		{1, {1, 0, 0}, {0,0,0}, {0,0,0}},
// 		{2, {0, 1, 0}, {0,0,0}, {0,0,0}}
// 	};

// 	// Create a face
// 	Face f;
// 	f.id = 0;
// 	f.idVertices = {0, 1, 2};
// 	P.faces.push_back(f);

// 	// Compute the barycenter and check if it's corrrect
// 	Vertex bc = Barycenter(P, 0);
// 	Vector3d expected = {1.0/3.0, 1.0/3.0, 0};
// 	ASSERT_TRUE(bc.coords.isApprox(expected, numeric_limits<double>::epsilon()));
// }


// // DualTest

// TEST(DualTest, EdgeNeighborsComputation)
// {
// 	// Create a polyhedron
// 	Polyhedron P = Octahedron();

// 	// Get vertex neighbors
// 	getEdgeNeighbors(P);

// 	// Test edge neighbors
// 	for (const auto& e : P.edges)
// 	{
// 		ASSERT_EQ(e.faceNeighbors.size(), 2);
// 	}
// }


// TEST(DualTest, VertexNeighborsComputation)
// {
// 	// Create a tetrahedron
// 	Polyhedron P = Tetrahedron();

// 	// Get vertex neighbors
// 	getEdgeNeighbors(P);
// 	getVertexNeighbors(P);

// 	// Test vertex 0's neighbors
// 	auto edgeNeighbors0 = P.vertices[0].edgeNeighbors;
// 	auto faceNeighbors0 = P.vertices[0].faceNeighbors;
// 	sort(edgeNeighbors0.begin(), edgeNeighbors0.end());
// 	sort(faceNeighbors0.begin(), faceNeighbors0.end());
// 	ASSERT_EQ(edgeNeighbors0, vector<unsigned int>({0, 2, 3}));
// 	ASSERT_EQ(faceNeighbors0, vector<unsigned int>({0, 1, 2}));

// 	// Test vertex 1's neighbors
// 	auto edgeNeighbors1 = P.vertices[1].edgeNeighbors;
// 	auto faceNeighbors1 = P.vertices[1].faceNeighbors;
// 	sort(edgeNeighbors1.begin(), edgeNeighbors1.end());
// 	sort(faceNeighbors1.begin(), faceNeighbors1.end());
// 	ASSERT_EQ(edgeNeighbors1, vector<unsigned int>({0, 1, 4}));
// 	ASSERT_EQ(faceNeighbors1, vector<unsigned int>({0, 2, 3}));

// 	// Test vertex 2's neighbors
// 	auto edgeNeighbors2 = P.vertices[2].edgeNeighbors;
// 	auto faceNeighbors2 = P.vertices[2].faceNeighbors;
// 	sort(edgeNeighbors2.begin(), edgeNeighbors2.end());
// 	sort(faceNeighbors2.begin(), faceNeighbors2.end());
// 	ASSERT_EQ(edgeNeighbors2, vector<unsigned int>({1, 2, 5}));
// 	ASSERT_EQ(faceNeighbors2, vector<unsigned int>({0, 1, 3}));

// 	// Test vertex 3's neighbors
// 	auto edgeNeighbors3 = P.vertices[3].edgeNeighbors;
// 	auto faceNeighbors3 = P.vertices[3].faceNeighbors;
// 	sort(edgeNeighbors3.begin(), edgeNeighbors3.end());
// 	sort(faceNeighbors3.begin(), faceNeighbors3.end());
// 	ASSERT_EQ(edgeNeighbors3, vector<unsigned int>({3, 4, 5}));
// 	ASSERT_EQ(faceNeighbors3, vector<unsigned int>({1, 2, 3}));
// }


// TEST(DualTest, DualComputation)
// {
// 	// Create a polyhedron object
// 	Polyhedron P = Tetrahedron();
// 	getEdgeNeighbors(P);
// 	getVertexNeighbors(P);
	
// 	// Compute the dual (tetrahedron)
// 	Polyhedron Q = Dual(P);

// 	// Check the number of vertices, edges and faces
// 	ASSERT_EQ(Q.numVertices(), P.numFaces());
// 	ASSERT_EQ(Q.numEdges(), P.numEdges());
// 	ASSERT_EQ(Q.numFaces(), P.numVertices());

// 	// Check that the vertices are normalized
// 	for (const auto& v : Q.vertices)
// 	{
// 		double length = v.coords.norm();
// 		ASSERT_NEAR(length, 1.0, numeric_limits<double>::epsilon());
// 	}

// 	// Check the number of vertices and edges in a face
// 	for (const auto& f : Q.faces)
// 	{
// 		ASSERT_EQ(f.idVertices.size(), 3);
// 		ASSERT_EQ(f.idEdges.size(), 3);
// 	}
	
// 	// Check if the dual is coherent
// 	ASSERT_TRUE(Q.checkFaces());
// }