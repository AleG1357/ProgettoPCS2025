#pragma once

#include "PolyhedralMesh.hpp"
#include "PolyhedralMeshBuilder.hpp"
#include "ProjectionUtilities.hpp"
#include <vector>
#include <cmath>

namespace PolygonalLibrary
{

// Costruisce un tetraedro regolare proiettato sulla sfera unitaria
void GenerateTetrahedron(PolyhedralMesh& mesh)
{
    mesh = PolyhedralMesh(); // Reset mesh (facoltativo se già vuota)

    // Vertici di un tetraedro regolare centrato nell'origine
    const double sqrt_3 = std::sqrt(3.0);
    std::vector<std::vector<double>> coords = {
        { 1,  1,  1},
        {-1, -1,  1},
        {-1,  1, -1},
        { 1, -1, -1}
    };

    // Inserimento vertici
    for (const auto& v : coords)
        AddCell0D(mesh, v[0], v[1], v[2]);

    // Facce (triangoli), definite come liste di vertici ordinati
    std::vector<std::vector<unsigned int>> faces = {
        {0, 1, 2},
        {0, 3, 1},
        {0, 2, 3},
        {1, 3, 2}
    };

    for (const auto& face : faces)
        AddCell2D(mesh, face);

    // Proiezione dei vertici sulla sfera unitaria
    ProjectVerticesOnUnitSphere(mesh);
}

}
