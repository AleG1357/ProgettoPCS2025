#pragma once

#include <iostream>
#include <vector>

#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


// Struct which stores the ID and coordinates of a vertex
struct Vertex
{
	unsigned int id = 0;
	Vector3d coords = Vector3d::Zero();
	vector<unsigned int> edgeNeighbors{}; // IDs of adjacent edges
	vector<unsigned int> faceNeighbors{}; // IDs of adjacent faces
	bool shortPath = false; /// per evidenziare i cammini minimi

	//
};

// Struct which stores the ID of an edge and the IDs of its extrema
struct Edge
{
	unsigned int id = 0;
	unsigned int origin = 0;
	unsigned int end = 0;
	vector<unsigned int> faceNeighbors{}; // IDs of adjacent faces
	bool shortPath = false;
};

// Struct which stores the ID of a face, the IDs of its vertices and edges
	// and the number of vertices and edges
struct Face
{
	unsigned int id = 0;
	vector<unsigned int> idVertices{};
	vector<unsigned int> idEdges{};

	// Method which returns the number of vertices of the face
	unsigned int numVertices() const
	{
		return idVertices.size();
	}

	// Method which returns the number of edges of the face
	unsigned int numEdges() const
	{
		return idEdges.size();
	}
};

// Struct which stores the ID of a polyhedron, the IDs of its vertices and edges
// and the number of vertices, edges and faces
struct Polyhedron
{
	unsigned int id = 0;
	vector<Vertex> vertices{};
	vector<Edge> edges{};
	vector<Face> faces{};

	// Method which returns the number of vertices of the polyhedron
	unsigned int numVertices() const
	{
		return vertices.size();
	}

	// Method which returns the number of edges of the polyhedron
	unsigned int numEdges() const
	{
		return edges.size();
	}

	// Method which returns the number of faces of the polyhedron
	unsigned int numFaces() const
	{
		return faces.size();
	}

	// Method which checks if the vertices and edges of each face are coherent
	bool checkFaces() const
	{
		// Iterate through all faces of the polyhedron
		for (const auto& face : faces)
		{
			// Get number of edges and vertices
			// (for this project we expect this value to be always 3)
			unsigned int E = face.numEdges();
			unsigned int V = face.numVertices();

			// Check if they are the same
			if(E != V)
			{
				cerr << "Mismatch number of edges and vertices" << endl;
				return false;
			}

			// Iterate along each vector of IDs
			for (size_t e = 0; e < E; ++e)
			{
				// Get IDs of the e-th edge and vertex, and the e-th edge
				unsigned int idEdge = face.idEdges[e];
				unsigned int idVertex = face.idVertices[e];

				// Get e-th edge
				const Edge& edge = edges[idEdge];

				// The ID of the e-th vertex must be either the origin or the end
				// of the e-th edge
				if (edge.origin != idVertex && edge.end != idVertex)
				{
					cerr << "Error: vertex-edge mismatch" << endl;
					return false;
				}

				// Get ID of the (e+1)-th vertex
				unsigned int idVertexNext = face.idVertices[(e + 1) % E];
				
				// Get ID of the end of the e-th edge
				unsigned int currentEdgeEnd;
				if (edge.origin == idVertex)
				{
					currentEdgeEnd = edge.end;
				}
				else
				{
					currentEdgeEnd = edge.origin;
				}

				// The ID of the (e+1)-th vertex must be the end of the e-th edge
				if (currentEdgeEnd != idVertexNext)
				{
					cerr << "Error: edge-edge discontinuity" << endl;
					return false;
				}
			}
		}
		
		return true;
	}
};