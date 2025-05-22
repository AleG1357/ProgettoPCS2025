#include "PolyhedralMesh.hpp"
#include "PolyhedralMeshBuilder.hpp"
#include "ProjectionUtilities.hpp"
#include "UCDUtilities.hpp"
#include "SolidGenerator.hpp"
#include "GeodesicGenerator.hpp"
#include "GeodesicSubdivision.hpp"

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

    if ((p != 3 && p != 4) || q < 3 || b < 0 || c < 0 || (b > 0 && c > 0))
    {
        cerr << "Input non valido. Deve essere p = 3 e (b = 0 o c = 0)." << endl;
        return 1;
    }

    PolyhedralMesh mesh;

    // === Riconoscimento e generazione ===
    if (b == 0 && c == 0)
    {
        DetectAndGeneratePlatonic(mesh, p, q);
    }
    else
    {
        GenerateGeodesicPolyhedronClassI(mesh, p, q, b + c);
    }

    // === Esportazione .txt ===
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

    // === .inp opzionale per Paraview ===
    UCDUtilities exporter;
    exporter.ExportPoints("Cell0Ds.inp", mesh.Cell0DsCoordinates);
    exporter.ExportSegments("Cell1Ds.inp", mesh.Cell0DsCoordinates, mesh.Cell1DsExtrema);
    exporter.ExportPolygons("Cell2Ds.inp", mesh.Cell0DsCoordinates, mesh.Cell2DsVertices);

    cout << "Solido generato con successo. File .txt e .inp pronti." << endl;
    return 0;
}
