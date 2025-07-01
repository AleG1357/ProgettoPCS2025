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
    if (argc < 5) {
        std::cerr << " Utilizzo corretto: " << argv[0] << " <p> <q> <b> <c> [<id_start> <id_end>]" <<endl;
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
        std::cerr << "Input error: " << ex.what() << std::endl;
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
		cout << " Classe II di parametro: " << val << endl;
		P = TriangleClassII(initial_P, val);
	}
	else {
		cerr << " b e c non validi " << endl;
		return 1;
	}


	
	// Projection on the sphere
	for(auto& v : P.vertices)
	{
		normalizeVertex(v);
	}
	
	
	// Get neighbors of each vertex and edge
	getEdgeNeighbors(P);
	getVertexNeighbors(P);


	// Create another polyhedron
	Polyhedron P_new;
	P_new = P;

	// Create the dual polyhedron, if needed
	if(p != 3)
	{
		P_new = Dual(P);

		// Get new neighbors
		getEdgeNeighbors(P_new);
		getVertexNeighbors(P_new);

	}


	// Shortest path 
	if(argc == 7)
	{
		// Initialize variables
		unsigned int id_path_start; // ID of starting vertex
		unsigned int id_path_end; // ID of ending vertex

		// Check input
		if (!checkGraphInput(argv, id_path_start, id_path_end, P_new))
		{
			return 1;
		}

		// Initialize graph and weight matrix
		Graph graph = createGraph(P_new);
		MatrixXd weights = createWeights(P_new);

		// Run Dijkstra algorithm
		vector<unsigned int> path = Dijkstra(graph, id_path_start, id_path_end, weights);

		// Draw path on the polyhedron
		drawPath(P_new, path);

		// Print path in the terminal
		printPath(P_new, path);

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