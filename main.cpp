#include "PolyhedralMesh.hpp"
#include "PolyhedralMeshBuilder.hpp"
#include "ProjectionUtilities.hpp"
#include "UCDUtilities.hpp"
#include "SolidGenerator.hpp"
#include "GeodesicGenerator.hpp"
#include "GeodesicSubdivision.hpp"
#include "DualMeshGenerator.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;
using namespace PolygonalLibrary;
using namespace Gedim;

int main(int argc, char** argv)
{
    if (argc != 5)
    {
        cerr << "Uso corretto: ./ProgettoPCS2025 p q b c" << endl;
        return 1;
    }

    int p = stoi(argv[1]);
    int q = stoi(argv[2]);
    int b = stoi(argv[3]);
    int c = stoi(argv[4]);

    if ((p != 3 && p != 4 && p != 5) || q < 3 || b < 0 || c < 0 || (b > 0 && c > 0))
    {
        cerr << "Input non valido. p ∈ {3,4,5}, q ≥ 3, b ≥ 0, c ≥ 0, ma non entrambi > 0." << endl;
        return 1;
    }

    PolyhedralMesh mesh;

    // === CASO 1: p = 3 → geodesico (classe I)
    if (p == 3)
    {
        GenerateGeodesicPolyhedronClassI(mesh, p, q, b + c);
    }

    // === CASO 2: q = 3 → poliedro di Goldberg (duale)
    else if (q == 3)
    {
       /*/ // eccezione: tetraedro → duale è se stesso
        if (p == 3 && q == 3)
        {                                 Ho commentato perchè per ora è ridondante
            GenerateTetrahedron(mesh);  
        }
        else
        {*/       
            // costruiamo il geodetico con p e q invertiti
            PolyhedralMesh base;
            GenerateGeodesicPolyhedronClassI(base, std::min(p, q), std::max(p, q), b + c);
            GenerateDualMesh(base, mesh);
        //}
    }


    // === RIMOZIONE DUPLICATI ===
    RemoveDuplicateVertices(mesh);
    RemoveDuplicateEdges(mesh);

    // === CHECK NUMERI TEORICI (debug)
    int T = b * b + b * c + c * c;
    int expectedV = 0, expectedE = 0, expectedF = 0;

    if (q == 3) {
        expectedV = 2 * T + 2;
        expectedE = 6 * T;
        expectedF = 4 * T;
    } else if (q == 4) {
        expectedV = 4 * T + 2;
        expectedE = 12 * T;
        expectedF = 8 * T;
    } else if (q == 5) {
        expectedV = 10 * T + 2;
        expectedE = 30 * T;
        expectedF = 20 * T;
    }

    std::cout << "🔍 Verifica struttura mesh:" << std::endl;
    std::cout << "   Attesi: V=" << expectedV << " E=" << expectedE << " F=" << expectedF << std::endl;
    std::cout << "   Ottenuti: V=" << mesh.NumCell0Ds << " E=" << mesh.NumCell1Ds << " F=" << mesh.NumCell2Ds << std::endl;





    // === ESPORTAZIONE .txt
    ofstream f0("Cell0Ds.txt");
    for (unsigned int i = 0; i < mesh.NumCell0Ds; ++i)
        f0 << mesh.Cell0DsId[i] << " "
           << mesh.Cell0DsCoordinates(0, i) << " "
           << mesh.Cell0DsCoordinates(1, i) << " "
           << mesh.Cell0DsCoordinates(2, i) << "\n";
    f0.close();

    ofstream f1("Cell1Ds.txt");
    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i)
        f1 << mesh.Cell1DsId[i] << " "
           << mesh.Cell1DsExtrema(0, i) << " "
           << mesh.Cell1DsExtrema(1, i) << "\n";
    f1.close();

    ofstream f2("Cell2Ds.txt");
    for (unsigned int i = 0; i < mesh.NumCell2Ds; ++i)
    {
        f2 << mesh.Cell2DsId[i] << " ";
        for (auto v : mesh.Cell2DsVertices[i]) f2 << v << ",";
        f2 << " ";
        for (auto e : mesh.Cell2DsEdges[i]) f2 << e << ",";
        f2 << "\n";
    }
    f2.close();

    ofstream f3("Cell3Ds.txt");
    mesh.Cell3DsId.resize(1);
    mesh.Cell3DsVertices.resize(1);
    mesh.Cell3DsEdges.resize(1);
    mesh.Cell3DsFaces.resize(1);

    for (unsigned int i = 0; i < mesh.NumCell0Ds; ++i)
        mesh.Cell3DsVertices[0].push_back(i);
    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i)
        mesh.Cell3DsEdges[0].push_back(i);
    for (unsigned int i = 0; i < mesh.NumCell2Ds; ++i)
        mesh.Cell3DsFaces[0].push_back(i);

    mesh.Cell3DsId[0] = 0;
    mesh.NumCell3Ds = 1;

    f3 << "0 ";
    for (auto v : mesh.Cell3DsVertices[0]) f3 << v << ",";
    f3 << " ";
    for (auto e : mesh.Cell3DsEdges[0]) f3 << e << ",";
    f3 << " ";
    for (auto f : mesh.Cell3DsFaces[0]) f3 << f << ",";
    f3 << "\n";
    f3.close();

    // === ESPORTAZIONE .inp
    UCDUtilities exporter;
    exporter.ExportPoints("Cell0Ds.inp", mesh.Cell0DsCoordinates);
    exporter.ExportSegments("Cell1Ds.inp", mesh.Cell0DsCoordinates, mesh.Cell1DsExtrema);
    exporter.ExportPolygons("Cell2Ds.inp", mesh.Cell0DsCoordinates, mesh.Cell2DsVertices);

    cout << "✅ Solido generato. File .txt e .inp pronti." << endl;
    return 0;
}
