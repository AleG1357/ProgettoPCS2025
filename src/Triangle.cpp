#include "Triangle.hpp"
#include "Geometry.hpp"


using namespace std;
using namespace Eigen;


// Function which adds missing vertices
unsigned int addVertexIfMissing(Polyhedron& P, const Vector3d coords_V)
{
	// Get machine precision
	const double eps = numeric_limits<double>::epsilon();

	// Iterate along existing vertices
	for (const auto& existing : P.vertices)
	{
		// Check if the vertex already exists
		if (coords_V.isApprox(existing.coords, eps))
		{
			// If it does, return its ID
			return existing.id;
		}
	}

	// Create the vertex if it doesn't exist yet
	Vertex v_new;
	
	// Set coordinates
	v_new.coords = coords_V;

	// Set correct ID (first available number)
	v_new.id = P.numVertices();

	// Add it to the polyhedron
	P.vertices.push_back(v_new);

	return v_new.id;
}


// Function which adds missing edges
unsigned int addEdgeIfMissing(Polyhedron& P, unsigned int id1, unsigned int id2, unsigned int id_e)
{
	// Check if vertices exists in the polyhedron
	if (id1 >= P.numVertices() || id2 >= P.numVertices())
	{
		cerr << "Error: invalid edge" << endl;
		return numeric_limits<unsigned int>::max(); // max unsigned int
	}
	
	// Check if the edge is valid
	if (id1 == id2)
	{
		cerr << "Error: degenerate edge" << endl;
		return numeric_limits<unsigned int>::max();
	}
	
	
	// Iterate along existing edges
	for (const auto& existing : P.edges)
	{
		// Check if the edge already exists
		if (((existing.origin == id1) && (existing.end == id2)) ||
			((existing.origin == id2) && (existing.end == id1)))
		{
			// If it does, return its ID
			return existing.id;
		}
	}

	// Create the edge if it doesn't exist yet
	Edge e_new;
	
	// Assign the correct ID
	if (id_e != numeric_limits<unsigned int>::max())
	{
		// Check if the ID is already used
		for (const auto& existing : P.edges)
		{
			if (existing.id == id_e)
			{
				cerr << "Error: forced ID already used" << endl;
				return numeric_limits<unsigned int>::max();
			}
		}

		// Set the requested ID
		e_new.id = id_e;
	}
	else
	{
		// Set the first available number
		e_new.id = P.numEdges();
	}

	// Set origin and end to current vertices
	e_new.origin = id1;
	e_new.end = id2;

	// Add it to the polyhedron
	P.edges.push_back(e_new);
	
	return e_new.id;
}


// Function for Class I triangulation of a polyhedron with parameter val (val > 0)
Polyhedron TriangleClassI(const Polyhedron& P_old, const unsigned int& val)
{
	// Initialize a new polyhedron
	Polyhedron P;
	
	// Assign the same ID as the old polyhedron
	P.id = P_old.id;

	// If val = 1, no triangulation is needed
	if (val == 1)
	{
		return P_old;
	}
	
	// Allocate the correct amount of space for the polyhedron

	// Temporary variable
	unsigned int T = val * val;

	// Tetrahedron
	if (P_old.id == 1) // id 0---->1
	{
		P.vertices.reserve(2 * T + 2);
		P.edges.reserve(6 * T);
		P.faces.reserve(4 * T);
	}
	// Octahedron
	else if (P_old.id == 2)  // id 1--->2
	{
		P.vertices.reserve(4 * T + 2);
		P.edges.reserve(12 * T);
		P.faces.reserve(8 * T);
	}
	// Icosahedron
	else
	{
		P.vertices.reserve(10 * T + 2);
		P.edges.reserve(30 * T);
		P.faces.reserve(20 * T);
	}


	// Iterate along faces of the platonic solid
	for (const auto& face : P_old.faces)
	{
		// We expect all faces to be triangles
		// Get IDs and a reference of each vertex of the face
		unsigned int id_A = face.idVertices[0];
		const Vertex& A = P_old.vertices[id_A];
		
		unsigned int id_B = face.idVertices[1];
		const Vertex& B = P_old.vertices[id_B];
		
		unsigned int id_C = face.idVertices[2];
		const Vertex& C = P_old.vertices[id_C];
		
		
		// Create new vertices

		// Auxiliary map that associates (i,j) indices with vertices' IDs
		map<pair<unsigned int, unsigned int>, unsigned int> Vij;

		// (i,j) are the barycentric coordinates
		for (unsigned int i = 0; i <= val; i++)
		{
			for (unsigned int j = 0; j <= val - i; j++)
			{
				// Compute new vertex coordinates
				Vector3d coords_V = (i * A.coords + j * B.coords + (val - i - j) * C.coords) / val;

				// If the vertex doesn't exist, add it
				unsigned int id_V = addVertexIfMissing(P, coords_V);
				
				// Save V's ID and indices in the map
				Vij.insert({{i, j}, id_V});
			}
		}
		
		// Create new edges and faces

		for (unsigned int i = 0; i <= val - 1; i++)
		{
			for( unsigned int j = 0; j <= val - 1 - i; j++)
			{
				// Add the "upside triangles"
				
				// Get IDs of the three vertices
				unsigned int id0v = Vij[{i, j}];
				unsigned int id1v = Vij[{i + 1, j}];
				unsigned int id2v = Vij[{i, j + 1}];
				
				// Add edges to the polyhedron
				unsigned int id0e0 = addEdgeIfMissing(P, id0v, id1v);
				unsigned int id1e0 = addEdgeIfMissing(P, id1v, id2v);
				unsigned int id2e0 = addEdgeIfMissing(P, id2v, id0v);

				// Initialize face
				Face f0;

				// ID is first available number
				f0.id = P.numFaces();

				// Add vertices to the face
				f0.idVertices = {id0v, id1v, id2v};

				// Add edges to the face
				f0.idEdges = {id0e0, id1e0, id2e0};
				
				// Add the face to the polyhedron
				P.faces.push_back(f0);
				
				// Check if the polyhedron is still coherent
				if (!P.checkFaces())
				{
					// If not, remove the face from the polyhedron and print error
					P.faces.pop_back();
					cerr << "Error: invalid polyhedron" << endl;

				}
				
				// Add the "downside triangles"
				
				if (i + j <= val - 2)
				{
					// Get ID of the other vertex
					unsigned int id3v = Vij[{i + 1, j + 1}];
				
					// Get IDs of the edges
					unsigned int id0e1 = addEdgeIfMissing(P, id1v, id2v);
					unsigned int id1e1 = addEdgeIfMissing(P, id2v, id3v);
					unsigned int id2e1 = addEdgeIfMissing(P, id3v, id1v);
				
					// Initialize face
					Face f1;
					
					// ID is first available number
					f1.id = P.numFaces();
				
					// Add vertices to the face
					f1.idVertices = {id1v, id2v, id3v};
					
					// Add edges to the face
					f1.idEdges = {id0e1, id1e1, id2e1};
				
					// Add the face to the polyhedron
					P.faces.push_back(f1);
				
					// Check if the polyhedron is still coherent
					if (!P.checkFaces())
					{
						// If not, remove the face from the polyhedron and print error
						P.faces.pop_back();
						cerr << "Error: invalid polyhedron" << endl;
					}
				}
			}
		}
	}


	return P;

}


// Function for Class II triangulation of a polyhedron with parameter a (b = c =: val > 0)
Polyhedron TriangleClassII(const Polyhedron& P_old, const unsigned int& val)
{
	// Initialize a new polyhedron
	Polyhedron P;
	
	// Assign the same ID as the old polyhedron
	P.id = P_old.id;
	

	// Execute class I triangulation of the old polyhedron
	Polyhedron P_I = TriangleClassI(P_old, val);
	
	// Get the face neighbors of the edges
	getEdgeNeighbors(P_I);


	// Create new vertices
	
	// Copy the vertices of the class I triangulation
	P.vertices = P_I.vertices;
	
	// Auxiliary map that associates the face of the original polyhedron with their barycenters
	unordered_map<unsigned int, unsigned int> f_bc;
	
	// Add the barycenters to the vertices of the polyhedron
	for (const auto& f : P_I.faces)
	{
		// Compute the barycenters of the original polyhedron's faces
		Vertex bc = Barycenter(P_I, f.id);
		
		// Assign the next available number as the ID
		bc.id = P.numVertices();
		
		// Add the barycenter to the map
		f_bc[f.id] = bc.id;
		
		// Add the barycenter to the polyhedron
		P.vertices.push_back(bc);
	}
	
	
	// Create new edges and faces
	
	// Iterate along the edges of the original polyhedron
	for (const auto& e : P_I.edges)
	{
		// Get its extrema (same IDs as the new polyhedron)
		unsigned int idV0 = e.origin;
		unsigned int idV1 = e.end;
		
		// Get the barycenters of the neighbor faces
		unsigned int idBC0 = f_bc[e.faceNeighbors[0]];
		unsigned int idBC1 = f_bc[e.faceNeighbors[1]];
		
		
		// First triangle: v0 - BC0 - BC1
		
		// Add the new edges
		unsigned int idE0 = addEdgeIfMissing(P, idV0, idBC0);
		unsigned int idE1 = addEdgeIfMissing(P, idBC0, idBC1);
		unsigned int idE2 = addEdgeIfMissing(P, idBC1, idV0);
		
		// Define the new face
		Face f0 = {P.numFaces(), {idV0, idBC0, idBC1}, {idE0, idE1, idE2}};
		
		// Add the new face
		P.faces.push_back(f0);
		
		
		// Second triangle: v1 - BC0 - BC1
		
		// Add the new edges
		unsigned int idE3 = addEdgeIfMissing(P, idBC0, idV1);
		unsigned int idE4 = addEdgeIfMissing(P, idV1, idBC1);
		
		// Define the new face
		Face f1 = {P.numFaces(), {idBC0, idV1, idBC1}, {idE3, idE4, idE1}};
		
		// Add the new face
		P.faces.push_back(f1);
	}


	return P;
	
}