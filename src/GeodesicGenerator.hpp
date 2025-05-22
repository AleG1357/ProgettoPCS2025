#pragma once

#include "PolyhedralMesh.hpp"
#include "PolyhedralMeshBuilder.hpp"
#include "SolidGenerator.hpp"
#include "GeodesicSubdivision.hpp"

#include <cmath>
#include <iostream>
#include <vector>

namespace PolygonalLibrary
{

void GenerateGeodesicPolyhedronClassI(PolyhedralMesh& mesh, int p, int q, int b)
{
    // Supportiamo ora: icosaedro {3,5}, ottaedro {3,4}, tetraedro {3,3}
    if (p != 3 || (q != 5 && q != 4 && q != 3))
    {
        std::cerr << "Errore: triangolazione classe I supportata solo per solidi triangolari {3,3}, {3,4}, {3,5}." << std::endl;
        return;
    }

    // Genera il solido platonico di base
    if (q == 5)
        GenerateIcosahedron(mesh);
    else if (q == 4)
        GenerateOctahedron(mesh);
    else if (q == 3)
        GenerateTetrahedron(mesh);

    // Se b > 1, triangola
    if (b > 1)
        TriangulateClassI(mesh, b);
}

}


