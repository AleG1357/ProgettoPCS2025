#include <map>
#include <fstream>
#include <sstream>
#include <numbers>

#include "Utils.hpp"
#include "Geometry.hpp"
#include "Polyhedron.hpp"


using namespace std;
using namespace Eigen;



Polyhedron buildPlatonicSolid(int p, int q)
{
	Polyhedron solid;
	
	// Gestione dei casi noti
	if(p == 3 && q == 3)
	{	solid.id = 1;
		
		double a = sqrt(3)/3;
		solid.vertices = {
			{0, Vector3d(a,a,a)}, {1, Vector3d(a,-a,-a)}, 
			{2, Vector3d(-a,a,-a)}, {3, Vector3d(-a,-a,a)}
		};

		solid.faces = {
			{0, {0,1,2}},
			{1, {0,3,1}},
			{2, {0,2,3}},
			{3, {1,3,2}}
		};
	}    

	else if ((p == 3 && q == 4) || (p == 4 && q == 3 )) // cubo e ottaedro 
		{   solid.id = 2;
			solid.vertices = {
				{0, Vector3d(1,0,0)}, {1, Vector3d(-1,0,0)}, 
				{2, Vector3d(0,1,0)}, {3, Vector3d(0,-1,0)},
				{4, Vector3d(0,0,1)}, {5, Vector3d(0,0,-1)}
			};

			solid.faces = {
				{0, {0,2,4}}, {1, {2,1,4}}, {2, {1,3,4}}, {3, {3,0,4}},
				{4, {2,0,5}}, {5, {1,2,5}}, {6, {3,1,5}}, {7, {0,3,5}}
			};
		}

	else if ((p == 3 && q == 5) || (p == 5 && q == 3)) //dodecaedro e icosaedro
		{   solid.id = 3;
			const double phi = (1 + sqrt(5))/2;
			double norm = sqrt(1 + phi*phi);

			vector<Vector3d> positions = {
				{0, 1, phi}, {0, -1, phi}, {0, 1, -phi}, {0, -1, -phi},
				{1, phi, 0}, {-1, phi, 0}, {1, -phi, 0}, {-1, -phi, 0},
				{phi, 0, 1}, {-phi, 0, 1}, {phi, 0, -1}, {-phi, 0, -1}
			};

			for(size_t i=0; i<positions.size(); i++)
				solid.vertices.push_back({static_cast<unsigned int>(i), positions[i]/norm});

			solid.faces = {
				{0,{0,1,8}}, {1,{0,8,4}}, {2,{0,4,5}}, {3,{0,5,9}}, {4,{0,9,1}},
				{5,{1,6,8}}, {6,{1,9,7}}, {7,{1,7,6}}, {8,{8,6,10}}, {9,{8,10,4}},
				{10,{4,10,2}}, {11,{4,2,5}}, {12,{5,2,11}}, {13,{5,11,9}},
				{14,{9,11,7}}, {15,{7,11,3}}, {16,{7,3,6}}, {17,{6,3,10}},
				{18,{10,3,2}}, {19,{2,3,11}}
			};
		}

		else {
			throw std::runtime_error("Solido non supportato.");
	}

	// COSTRUZIONE PRECISA E CHIARA DEI LATI (SPIGOLI):
	map<pair<unsigned int,unsigned int>, unsigned int> edgeIndexMap;
	unsigned int edgeId = 0;

	for (auto& face : solid.faces)
	{
		face.idEdges.clear();

		for (unsigned int j=0; j<face.idVertices.size(); ++j)
		{
			unsigned int vStart = face.idVertices[j];
			unsigned int vEnd = face.idVertices[(j+1)%3];

			pair<unsigned int,unsigned int> currentEdge = minmax(vStart, vEnd);

			// Se il lato non esiste, lo creo
			if(edgeIndexMap.find(currentEdge) == edgeIndexMap.end())
			{
				solid.edges.push_back({edgeId, currentEdge.first, currentEdge.second});
				edgeIndexMap[currentEdge] = edgeId;
				face.idEdges.push_back(edgeId);
				edgeId++;
			}
			else
			{
				face.idEdges.push_back(edgeIndexMap[currentEdge]);
			}
		}
	}

	// Restituisci il poliedro completo di vertici, spigoli e facce
	return solid;
}



// Function which allows to export a polyhedron for Paraview
void exportPolyhedron(const Polyhedron& P)
{   
	//cout << ">>> Entrato in exportPolyhedron" << endl;

	// Matrice coordinate vertici
	MatrixXd coordsCell0D = MatrixXd::Zero(3, P.numVertices());
	for (const auto& v : P.vertices)
		coordsCell0D.col(v.id) = v.coords;

	// Matrice estremi degli edge
	MatrixXi extremaCell1D = MatrixXi::Zero(2, P.numEdges());
	for (unsigned int i = 0; i < P.edges.size(); ++i) {
		extremaCell1D(0, i) = P.edges[i].origin;
		extremaCell1D(1, i) = P.edges[i].end;
	}



	// Highlight path on the polyhedron

	// Vettore per evidenziare i vertici sul cammino
	vector<double> visitedNodes(P.numVertices(), 0.0);
	for (unsigned int i = 0; i < P.numVertices(); ++i)
		visitedNodes[i] = P.vertices[i].shortPath ? 1.0 : 0.0;

	// Initialize UCDProperty struct
	Gedim::UCDProperty<double> visitedNodes_UCD;
	visitedNodes_UCD.NumComponents = 1;

	// Set the attributes
	const double* ptr1 = visitedNodes.data();
	visitedNodes_UCD.Data = ptr1;
	visitedNodes_UCD.Label = "Nodi Visitati";

	// Initialize vector of UCDProperty structs and the struct itself
	vector<Gedim::UCDProperty<double>> points_properties = { visitedNodes_UCD };


	// Vettore per evidenziare gli edge sul cammino
	vector<double> visitedEdges(P.numEdges(), 0.0);
	for (unsigned int i = 0; i < P.numEdges(); ++i)
		visitedEdges[i] = P.edges[i].shortPath ? 1.0 : 0.0;



	// Initialize UCDProperty struct
	Gedim::UCDProperty<double> visitedEdges_UCD;
	visitedEdges_UCD.NumComponents = 1;

	// Set the attributes
	const double* ptr2 = visitedEdges.data();
	visitedEdges_UCD.Data = ptr2;
	visitedEdges_UCD.Label = "ShortPath";
	
	// Initialize vector of UCDProperty struct and the struct itself
	vector<Gedim::UCDProperty<double>> segments_properties = { visitedEdges_UCD };


	// Create UCDUtilities struct
	Gedim::UCDUtilities utilities;

	// Call export functions
	utilities.ExportPoints("./Cell0Ds.inp",
							coordsCell0D,
							points_properties);

	utilities.ExportSegments("./Cell1Ds.inp",
							 coordsCell0D,
							 extremaCell1D,
							 points_properties,
							 segments_properties);
}


// Function which writes output files in the requested format
bool writeOutput(const Polyhedron& P)
{
	// Esporta i vertici (0D)
	ofstream ofs_cell0D("Cell0Ds.txt");
	if (!ofs_cell0D) {
		cerr << "File Cell0Ds.txt cannot be created." << endl;
		return false;
	}
	ofs_cell0D << "id;X;Y;Z" << endl;
	for (const auto& v : P.vertices)
		ofs_cell0D << v.id << ";" << v.coords[0] << ";" << v.coords[1] << ";" << v.coords[2] << endl;
	ofs_cell0D.close();


	// Esporta gli edge (1D)
	ofstream ofs_cell1D("Cell1Ds.txt");
	if (!ofs_cell1D) {
		cerr << "File Cell1Ds.txt cannot be created." << endl;
		return false;
	}
	ofs_cell1D << "id;origin;end" << endl;
	for (const auto& e : P.edges)
		ofs_cell1D << e.id << ";" << e.origin << ";" << e.end << endl;
	ofs_cell1D.close();


	// Esporta le facce (2D)
	ofstream ofs_cell2D("Cell2Ds.txt");
	if (!ofs_cell2D) {
		cerr << "File Cell2Ds.txt cannot be created." << endl;
		return false;
	}
	ofs_cell2D << "id;NumVertices;Vertices;NumEdges;Edges" << endl;
	for (const auto& f : P.faces) {
		ofs_cell2D << f.id << ";" << f.numVertices() << ";";
		for (unsigned int i = 0; i < f.numVertices(); ++i)
			ofs_cell2D << f.idVertices[i] << ";";
		ofs_cell2D << f.numEdges() << ";";
		for (unsigned int i = 0; i < f.numEdges(); ++i) {
			ofs_cell2D << f.idEdges[i];
			if (i < f.numEdges() - 1) ofs_cell2D << ";";
		}
		ofs_cell2D << endl;
	}
	ofs_cell2D.close();


	// Esporta il solido (3D)
	ofstream ofs_cell3D("Cell3Ds.txt");
	if (!ofs_cell3D) {
		cerr << "File Cell3Ds.txt cannot be created." << endl;
		return false;
	}
	ofs_cell3D << "id;NumVertices;Vertices;NumEdges;Edges;NumFaces;Faces" << endl;
	ofs_cell3D << P.id << ";" << P.numVertices() << ";";
	for (unsigned int i = 0; i < P.numVertices(); ++i)
		ofs_cell3D << P.vertices[i].id << ";";
	ofs_cell3D << P.numEdges() << ";";
	for (unsigned int i = 0; i < P.numEdges(); ++i)
		ofs_cell3D << P.edges[i].id << ";";
	ofs_cell3D << P.numFaces() << ";";
	for (unsigned int i = 0; i < P.numFaces(); ++i) {
		ofs_cell3D << P.faces[i].id;
		if (i < P.numFaces() - 1) ofs_cell3D << ";";
	}
	ofs_cell3D << endl;
	ofs_cell3D.close();
	return true;

}