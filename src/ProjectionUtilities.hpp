#pragma once

#include "PolyhedralMesh.hpp"
#include <cmath>

namespace PolygonalLibrary
{

void ProjectVerticesOnUnitSphere(PolyhedralMesh& mesh)
{
    for (unsigned int i = 0; i < mesh.NumCell0Ds; ++i)
    {
        double x = mesh.Cell0DsCoordinates(0, i);
        double y = mesh.Cell0DsCoordinates(1, i);
        double z = mesh.Cell0DsCoordinates(2, i);

        double norm = std::sqrt(x * x + y * y + z * z);
        if (norm == 0.0) continue;

        mesh.Cell0DsCoordinates(0, i) = x / norm;
        mesh.Cell0DsCoordinates(1, i) = y / norm;
        mesh.Cell0DsCoordinates(2, i) = z / norm;
    }
}

}
