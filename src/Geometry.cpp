#include "Geometry.hpp"
#include "Triangle.hpp"

#include <map>
#include <algorithm>

using namespace std;
using namespace Eigen;


// Proietta le coordinate di un vertice sulla sfera unitaria
void projectOnUnitSphere(Vertex& v) {
	double norm = v.coords.norm();
	if (norm > 0) v.coords /= norm;
}


// Calcola il centroide di una faccia
Vertex faceCentroid(const Polyhedron& poly, unsigned int faceId) {
	Vertex centroid;
	centroid.coords = {0, 0, 0};
	centroid.id = faceId;
	unsigned int n = poly.faces[faceId].numVertices();
	for (unsigned int i = 0; i < n; ++i) {
		centroid.coords += poly.vertices[poly.faces[faceId].idVertices[i]].coords;
	}
	centroid.coords /= n;
	return centroid;
}



// Assegna le facce adiacenti a ciascun edge
void assignEdgeAdjacencies(Polyhedron& poly) {
	for (auto& edge : poly.edges) {
		edge.faceNeighbors.clear();
		for (const auto& face : poly.faces) {
			if (std::find(face.idEdges.begin(), face.idEdges.end(), edge.id) != face.idEdges.end()) {
				edge.faceNeighbors.push_back(face.id);
				if (edge.faceNeighbors.size() == 2) break;
			}
		}
	}
}


// Assegna le facce e gli edge adiacenti a ciascun vertice
void assignVertexAdjacencies(Polyhedron& poly) {
	std::unordered_map<unsigned int, std::vector<unsigned int>> edgeMap;
	for (const auto& edge : poly.edges) {
		edgeMap[edge.origin].push_back(edge.id);
		edgeMap[edge.end].push_back(edge.id);
	}
	for (auto& v : poly.vertices) {
		v.faceNeighbors.clear();
		v.edgeNeighbors.clear();
		auto& neighbors = edgeMap[v.id];
		if (neighbors.empty()) continue;
		unsigned int startEdge = neighbors[0];
		unsigned int currentEdge = startEdge;
		unsigned int currentFace = poly.edges[startEdge].faceNeighbors[0];
		v.faceNeighbors.push_back(currentFace);
		unsigned int firstEdge = 0;
		for (unsigned int e_id : poly.faces[currentFace].idEdges) {
			if (e_id == startEdge) continue;
			if (std::find(neighbors.begin(), neighbors.end(), e_id) == neighbors.end()) continue;
			firstEdge = e_id;
			v.edgeNeighbors.push_back(firstEdge);
			break;
		}
		while (firstEdge != startEdge) {
			unsigned int nextFace = 0;
			for (unsigned int f_id : poly.edges[firstEdge].faceNeighbors) {
				if (std::find(v.faceNeighbors.begin(), v.faceNeighbors.end(), f_id) != v.faceNeighbors.end()) continue;
				nextFace = f_id;
				v.faceNeighbors.push_back(nextFace);
				break;
			}
			unsigned int nextEdge = 0;
			for (unsigned int e_id : poly.faces[nextFace].idEdges) {
				if (std::find(v.edgeNeighbors.begin(), v.edgeNeighbors.end(), e_id) != v.edgeNeighbors.end()) continue;
				if (std::find(neighbors.begin(), neighbors.end(), e_id) == neighbors.end()) continue;
				nextEdge = e_id;
				v.edgeNeighbors.push_back(nextEdge);
				break;
			}
			firstEdge = nextEdge;
		}
	}
}


// Costruisce il poliedro duale
Polyhedron makeDualPolyhedron(const Polyhedron& poly) {
	Polyhedron dual;
	dual.id = poly.id + 2;
	dual.vertices.reserve(poly.numFaces());
	dual.edges.reserve(poly.numEdges());
	dual.faces.reserve(poly.numVertices());
	for (const auto& face : poly.faces) {
		Vertex v_dual = faceCentroid(poly, face.id);
		projectOnUnitSphere(v_dual);
		dual.vertices.push_back(v_dual);
	}
	for (const auto& edge : poly.edges) {
		addEdgeIfMissing(dual, edge.faceNeighbors[0], edge.faceNeighbors[1], edge.id);
	}
	for (const auto& v : poly.vertices) {
		Face f_dual;
		f_dual.id = v.id;
		for (size_t i = 0; i < v.faceNeighbors.size(); ++i) {
			f_dual.idVertices.push_back(v.faceNeighbors[i]);
			f_dual.idEdges.push_back(v.edgeNeighbors[i]);
		}
		dual.faces.push_back(f_dual);
	}
	return dual;
}