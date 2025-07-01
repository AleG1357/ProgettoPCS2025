#include "Graph.hpp"


using namespace std;
using namespace Eigen;


// Function which creates the graph 
Graph createGraph(const Polyhedron& pol)
{
    // Initialize graph struct
    Graph graph;

    // Allocate correct amount of space
    // Use resize to avoid accessing invalid memory
    graph.adjacencyList.resize(pol.numVertices());

    // Iterate along vertices to fill the adjacency list
    for (const auto& v : pol.vertices)
    {
        // Iterate along the vertex's edge neighbors
        for (unsigned int i = 0; i < v.edgeNeighbors.size(); ++i)
        {
            // If the current vertex is the edge's origin
            if (pol.edges[v.edgeNeighbors[i]].origin == v.id)
            {
                // Add the edge's end
				unsigned int tmp = pol.edges[v.edgeNeighbors[i]].end;
                graph.adjacencyList[v.id].push_back(tmp);
            }
            else
            {
                // Add the edge's origin
                unsigned int tmp = pol.edges[v.edgeNeighbors[i]].origin;
				graph.adjacencyList[v.id].push_back(tmp);
            }
        }
    }


    return graph;

}


// Create the weights matrix
MatrixXd createWeights(const Polyhedron& pol)
{
    // Initialize the matrix to be "infinity" for all weights
	MatrixXd weights = MatrixXd::Constant(pol.numVertices(), pol.numVertices(), numeric_limits<double>::max());
	
	// Initialize diagonal elements (distance to self = 0)
	for(unsigned int i = 0; i < pol.numVertices(); ++i)
    {
        weights(i, i) = 0.0;
    }

    // Iterate along edges to set the weights
    for (const auto& e: pol.edges)
    {

        /////////////////////////////////////////////

        // Weighted graph

        // Get extrema of current edge
        Vertex v1 = pol.vertices[e.origin];
        Vertex v2 = pol.vertices[e.end];

        // Compute the edge's length
        double length = (v1.coords - v2.coords).norm();

        // Set the weight equal to the length
        weights(e.origin, e.end) = length;
        weights(e.end, e.origin) = length;

        /////////////////////////////////////////////

        // Unweighted graph

        // weights(e.origin, e.end) = 1.0;
        // weights(e.end, e.origin) = 1.0;
    }


    return weights;

}


// Dijkstra algorithm, returns vector of IDs of visited vertices
vector<unsigned int> Dijkstra(const Graph& graph,
                                const unsigned int& id_path_start,
                                const unsigned int& id_path_end,
                                const MatrixXd& weights)
{
    

    // Initialize vectors 
    vector<int> pred; // signed int initialized to -1
    vector<double> dist; // doubles initialized to "infinity"

    // Get number of nodes
    unsigned int N = graph.adjacencyList.size();

    // Reserve correct amount of space
    // Use resize (since we access the elements, they need to be initialized)
    pred.resize(N);
    dist.resize(N);

    // Fill the vectors with initial values
    for (unsigned int i = 0; i < N; i++)
    {
        pred[i] = -1;
        dist[i] = numeric_limits<double>::max();
    }

    // Initialize a priority queue
    // Each element is a pair (distance, vertex)
    // When .top() is called it returns the pair with shortest distance
    priority_queue<pair<double, unsigned int>,
                    vector<pair<double, unsigned int>>,
                    greater<pair<double, unsigned int>>> PQ;


    // Initialize source node
    pred[id_path_start] = id_path_start;
    dist[id_path_start] = 0.0;
    PQ.push({0.0, id_path_start});

    
    // Algorithm stops when the priority queue is empty
    while(!PQ.empty())
    {
        // Access element with lowest priority (distance)
        // d_v = (distance, vertex)
        pair<double, unsigned int> d_v = PQ.top();

        // Get current distance and node
        unsigned int current_dist = d_v.first;
        unsigned int current_node = d_v.second;

        // Then, dequeue it
        PQ.pop();

        // Stop the algorithm as soon as the end node is reached
        // (we only care about reaching this particular vertex)
        if (current_node == id_path_end)
        {
            break;
        }

        // If the node was already reached with a shorter path, skip
        if (current_dist > dist[current_node])
        {
            continue;
        }

        // For each node adjacent to u
        for (const auto& candidate : graph.adjacencyList[current_node])
        {
            // Compute new distance
            double new_dist = dist[current_node] + weights(current_node, candidate);

            // If the new distance is shorter
            if (dist[candidate] > new_dist)
            {
                // Update vector of distances
                dist[candidate] = new_dist;

                // Set current node's predecessor to u
                pred[candidate] = current_node;

                // Add it to the priority queue
                PQ.push({dist[candidate], candidate});
            }
        }
    }


    // Initialize requested path
    vector<unsigned int> path;

    // If node was not reached
    if (dist[id_path_end] == numeric_limits<double>::max())
    {
        cerr << "Something went wrong: no path found" << endl;
        return path; // empty vector
        
    }


    // Iterate following the sequence in the pred vector
    for (unsigned int i = id_path_end; i != id_path_start; i = pred[i])
    {
        path.push_back(i);
    }

    // Add starting node ("last predecessor") and reverse the order
    path.push_back(id_path_start);
    reverse(path.begin(), path.end());

    
    return path;

}


// Function which prints the path in the terminal
void printPath(Polyhedron& pol, const vector<unsigned int> path)
{

    for (unsigned int i = 0; i + 1 < path.size(); i++)
    {
        // Iterate along edges
        for (const auto& e : pol.edges)
        {
            if (((e.origin == path[i]) && (e.end == path[i + 1])) || 
                ((e.origin == path[i + 1]) && (e.end == path[i])))
            {
                cout << "Travelled across edge " << e.id 
                << " which connects nodes " << path[i] << " and " << path[i + 1] << endl;

                // Uncomment to check if shortPath was set to true
                //cout << e.shortPath << endl;
            }
        }
    }

    // Initialize total distance and number of crossed edges
    double path_length = 0.0;
    unsigned int edge_counter = 0;

    // Iterate along edges
    for (const auto& e : pol.edges)
    {
        if (e.shortPath == true)
        {
            // Get extrema of current edge
            Vertex v1 = pol.vertices[e.origin];
            Vertex v2 = pol.vertices[e.end];

            // Add edge length to total distance
            path_length += (v1.coords - v2.coords).norm();

            // Increment counter
            edge_counter++;
        }
    }


    // Print edge(s) crossed
    cout << "The path crosses " << edge_counter;
    if (edge_counter == 1)
    {
        cout << " edge"  << endl;  
    }
    else
    {
        cout << " edges" << endl;
    }
    
    // Print path length
    cout << "Total distance travelled is " << path_length << endl;

}


// Function which draws the path on the polyhedron
void drawPath(Polyhedron& pol, const vector<unsigned int> path)
{
    // Set short path flag to 1 for visited nodes
    for(unsigned int i = 0; i < path.size(); i++)
    {
        pol.vertices[path[i]].shortPath = true;
    }

    // Set short path flag to 1 for visited edges
    for (unsigned int i = 0; i + 1 < path.size(); i++)
    {
        // Get IDs of two consecutive nodes
        unsigned int node_1 = path[i];
        unsigned int node_2 = path[i + 1];

        // Find the edge connecting the two nodes
        for (auto& e : pol.edges)
        {
            // If the edge has origin and end equal to the current nodes
            if (((e.origin == node_1) && (e.end == node_2)) ||
            ((e.origin == node_2) && (e.end == node_1)))
            {
                // Set shortPath to true
                e.shortPath = true;
            } 
        }
    }
}