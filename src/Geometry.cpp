#include "Geometry.hpp"
#include "Triangle.hpp"


using namespace std;
using namespace Eigen;


// Function that normalizes the coordinates of a vertex to lie on the unit sphere
void normalizeVertex(Vertex& v)
{
	// Compute the Euclidean norm of the vector
	double length = v.coords.norm();

	// Normalize the coordinates
	v.coords /= length;
}


// Function which returns the barycenter of a face
Vertex Barycenter(const Polyhedron& P, const unsigned int& f_id)
{
	// Initialize a vertex
	Vertex bc;
	bc.coords = {0, 0, 0};

	// Assign same ID as the face
	bc.id = f_id;
	
	// Get number of vertices (we expect always 3)
	unsigned int num_v = P.faces[f_id].numVertices();

	//Compute coordinates of bc
	for (size_t i = 0; i < num_v; i++)
	{
		// ID of barycenter is the same as the ID of the face
		unsigned int id_v = P.faces[f_id].idVertices[i];

		// Running sum
		bc.coords += P.vertices[id_v].coords;
	}

	// Divide by number of vertices to take the average
	bc.coords /= num_v;


	return bc;

}



// Function which finds which faces each edge is adjacent to
void getEdgeNeighbors(Polyhedron& P)
{
	// For each edge
	for (auto& e : P.edges)
	{
		// Clear existing neighbors
		// (needed in case the function is called multiple times)
		e.faceNeighbors.clear();
		
		// Find faces containing current edge
		for (const auto& f : P.faces)
		{
			if (find(f.idEdges.begin(), f.idEdges.end(), e.id) != f.idEdges.end())
			{
				// Add face to neighbors
				e.faceNeighbors.push_back(f.id);
				
				// Each edge should connect exactly 2 faces
				if (e.faceNeighbors.size() == 2)
				{
					break;
				}
			}
		}
	}
}


// Function which finds which faces and edges each vertex is adjacent to
void getVertexNeighbors(Polyhedron& P)
{
	// Map that collects each vertex's neighbor edges
	unordered_map<unsigned int, vector<unsigned int>> unordered_neighbors;
	
	// Iterate on the polyhedron's edges to fill the map
	for (const auto& e : P.edges)
	{
		// Add each edge to the neighborhood of its origin and end
		unordered_neighbors[e.origin].emplace_back(e.id);
		unordered_neighbors[e.end].emplace_back(e.id);
	}
	
	// Iterate on the polyhedron's vertices
	for (auto& v : P.vertices)
	{
		// Clear existing neighbors
		v.faceNeighbors.clear();
		v.edgeNeighbors.clear();
		
		
		// Get current vertex's edge neighbors
		vector<unsigned int>& neighbors = unordered_neighbors[v.id];
		
		// Choose the starting edge (the first of the neighbors)
		unsigned int startEdgeId = neighbors[0];
		Edge startEdge = P.edges[startEdgeId];
		
		// Choose the starting face between the two neighbors of the first edge (the first)
		   // The starting face is also the first face to be added
		unsigned int currentFaceId = startEdge.faceNeighbors[0];
		
		// Add the first face to the face neighbors of the vertex
		v.faceNeighbors.push_back(currentFaceId);
		
		// Initialize the ID of the first edge to be added
		unsigned int currentEdgeId;
		
		// Find the first edge between the edges of the first face
		for (unsigned int e_id : P.faces[currentFaceId].idEdges) // 3 cycles
		{
			// Skip the starting edge
			if (e_id == startEdgeId)
			{
				continue;
			}
			
			// Skip the edge that doesn't contain the vertex
			if (find(neighbors.begin(), neighbors.end(), e_id) == neighbors.end())
			{
				continue;
			}
			
			// The only edge that remains is the correct one (the first edge)

			// Save the ID of the first edge
			currentEdgeId = e_id;
			
			// Add the first edge to the edge neighbors of the vertex
			v.edgeNeighbors.push_back(currentEdgeId);
			
			break;
		}
		
		// currentFaceId and currentEdgeId are the IDs of the first face and edge
		
		// Iterate until the cycle is closed, i.e. until the the current edge is the starting edge
		while (currentEdgeId != startEdgeId)
		{
			// Initialize the ID of the next face to be added
			unsigned int nextFaceId;
			
			// Find the next face between the two face neighbors of the edge
			for (unsigned int f_id : P.edges[currentEdgeId].faceNeighbors) // 2 cycles
			{
				// Exclude the face which has already been added
				if (find(v.faceNeighbors.begin(), v.faceNeighbors.end(), f_id) != v.faceNeighbors.end())
				{
					continue;
				}
				
				// The only face that remains is the correct one (the next face)
				
				// Save the ID of the next face
				nextFaceId = f_id;
				
				// Add the next face to the face neighbors of the vertex
				v.faceNeighbors.push_back(nextFaceId);
				
				break;
			}
			
			// Initialize the ID of the next edge to be added
			unsigned int nextEdgeId;
			
			// Find the next edge between the edges of the next face
			for (unsigned int e_id : P.faces[nextFaceId].idEdges) // 3 cycles
			{
				// Exclude the edge which has already been added
				if (find(v.edgeNeighbors.begin(), v.edgeNeighbors.end(), e_id) != v.edgeNeighbors.end())
				{
					continue;
				}
				
				// Exclude the edge that doesn't contain the vertex
				if (find(neighbors.begin(), neighbors.end(), e_id) == neighbors.end())
				{
					continue;
				}
				
				// The only edge that remains is the correct one (the next edge)
				
				// Save the ID of the next edge
				nextEdgeId = e_id;
				
				// Add the next edge to the edge neighbors of the vertex
				v.edgeNeighbors.push_back(nextEdgeId);
				
				break;
			}
			
			// Update the variables
			currentFaceId = nextFaceId;
			currentEdgeId = nextEdgeId;
		}
		
	}
	
}


// Function that creates the dual of a polyhedron
Polyhedron Dual(const Polyhedron& P)
{
	// Initialize the dual polyhedron
	Polyhedron Q;
	Q.id = P.id + 2;
	
	// Reserve space
	Q.vertices.reserve(P.numFaces());
	Q.edges.reserve(P.numEdges());
	Q.faces.reserve(P.numVertices());
	
	// Create vertices of the dual (one for each face of P)
	for (const auto& f : P.faces)
	{
		Vertex v_dual = Barycenter(P, f.id);
		normalizeVertex(v_dual);
		Q.vertices.push_back(v_dual);
	}
	
	// Create edges of the dual (one for each edge of P)
	for (const auto& e : P.edges)
	{
		// The face neighbors of the edge become the vertices of the dual edge
		addEdgeIfMissing(Q, e.faceNeighbors[0], e.faceNeighbors[1], e.id);
	}
	
	// Create faces of the dual (one for each vertex of P)
	for (const auto& v : P.vertices)
	{
		// Initialize a face, its ID is the corresponding vertex's
		Face f_dual;
		f_dual.id = v.id;
		
		// Add vertices and edges in the order they appear around the vertex
		// This maintains the ordering required by checkFaces
		for (size_t i = 0; i < v.faceNeighbors.size(); ++i)
		{
			f_dual.idVertices.push_back(v.faceNeighbors[i]);
			f_dual.idEdges.push_back(v.edgeNeighbors[i]);
		}
		
		// Add the face to the dual polyhedron
		Q.faces.push_back(f_dual);
	}
	

	return Q;
	
}