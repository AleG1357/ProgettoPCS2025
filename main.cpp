#include "Utils.hpp"
#include "Triangle.hpp"
#include "Geometry.hpp"
#include "Graph.hpp"


using namespace std;


// To pass parameters to main the standard syntax is:
// argc: number of arguments
// argv: array of arguments
// The first argument is always the executable file's name

int main(int argc, char* argv[]) {
	if (argc != 5 && argc != 7) {
		cerr << "Utilizzo corretto: " << argv[0] << " <p> <q> <b> <c> [<id_start> <id_end>]" << endl;
		return 1;
	}

	int tmp_p = std::stoi(argv[1]);
	int tmp_q = std::stoi(argv[2]);
	int tmp_b = std::stoi(argv[3]);
	int tmp_c = std::stoi(argv[4]);

	if (tmp_p < 0 || tmp_q < 0 || tmp_b < 0 || tmp_c < 0) {
		cerr << " Errore: tutti i parametri devono essere >= 0!" << std::endl;
		return 1;
	}

	unsigned int p = static_cast<unsigned int>(tmp_p);
	unsigned int q = static_cast<unsigned int>(tmp_q);
	unsigned int b = static_cast<unsigned int>(tmp_b);
	unsigned int c = static_cast<unsigned int>(tmp_c);

	// Determina classe e parametro di suddivisione
	unsigned int val = 0;
	unsigned int flag = 0; // 1=Classe I, 2=Classe II
	
	// unsigned int id_start = (argc > 5) ? std::stoi(argv[5]) : 0;
	// unsigned int id_end   = (argc > 6) ? std::stoi(argv[6]) : 0;

	Polyhedron initial_P;

	try {
		initial_P = buildPlatonicSolid(p, q);
	} catch (const std::exception& ex) {
		cerr << " Input error: p e q non validi \n Utilizzo corretto: [ (p,q) ∈ {3,4,5} ] e [ p = 3 o q = 3 ] " << ex.what() << endl;
		return 1;
	}

	Polyhedron P;
	
	//To triangulate the polyhedron
	if((b >= 1 && c == 0) || (b == 0 && c >=1)){
		flag = 1; //classe 1
		val = max(b,c);
		cout << " Classe I di parametro: " << val << endl;
		P = TriangleClassI(initial_P, val);
	}
	else if (b == c){
		flag = 2;
		val = b;
		cout << "Classe II di parametro: " << val << endl;
		P = TriangleClassII(initial_P, val);
	} else {
		cerr << "b e c non validi" << endl;
		return 1;
	}


	
	// Projection on the sphere
	for(auto& v : P.vertices)
	{
		projectOnUnitSphere(v);
	}
	
	
	// Get neighbors of each vertex and edge
	assignEdgeAdjacencies(P);
	assignVertexAdjacencies(P);


	// Create another polyhedron
	Polyhedron P_new;
	P_new = P;

	// Create the dual polyhedron, if needed
	if(p != 3)
	{
		P_new = makeDualPolyhedron(P);

		// Get new neighbors
		assignEdgeAdjacencies(P_new);
		assignVertexAdjacencies(P_new);

	}


	// Shortest path 
	if(argc == 7)
	{
		unsigned int id_path_start = std::stoi(argv[5]);
		unsigned int id_path_end = std::stoi(argv[6]);
		if (id_path_start >= P_new.numVertices() || id_path_end >= P_new.numVertices()) {
			cerr << "Out of bounds IDs" << endl;
			return 1;
		}
		if (id_path_start == id_path_end) {
			cerr << "Error: starting and ending IDs are equal" << endl;
			return 1;
		}

		// Costruisci grafo e matrice pesi
		Graph graph = buildAdjacencyGraph(P_new);
		MatrixXd weights = buildEdgeWeightMatrix(P_new);

		// Scegli algoritmo in base alla classe (flag==1: BFS, flag==2: Dijkstra)
		bool isUniformEdgeLength = (flag == 1);
		vector<unsigned int> path = findShortestPath(graph, id_path_start, id_path_end, isUniformEdgeLength, weights);

		// Evidenzia e stampa il percorso
		highlightPath(P_new, path);
		printPath(P_new, path);
		cout << " Percorso trovato tra i vertici " << id_path_start << " e " << id_path_end << endl;
	}


	// Export the polyhedron for Paraview
	exportPolyhedron(P_new);
		
	// Write .txt output files
	if(!writeOutput(P_new))
	{
		return 1;
	}

	
	return 0;
	
}