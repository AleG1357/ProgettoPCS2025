#pragma once

#include "PolyhedralMesh.hpp"
#include "PolyhedralMeshBuilder.hpp"
#include <Eigen/Dense>
#include <map>
#include <cmath>

namespace PolygonalLibrary
{

void TriangulateClassI(PolyhedralMesh& mesh, int b)
{
    if (b < 2)
        return; // niente da fare: T=1 = triangolo originale

    PolyhedralMesh newMesh;
    unsigned int currentVertexId = 0;

    // mappa per evitare duplicazioni
    std::map<std::tuple<unsigned int, unsigned int, unsigned int>, unsigned int> vertexMap;

    for (unsigned int f = 0; f < mesh.NumCell2Ds; ++f)
    {
        const auto& V = mesh.Cell2DsVertices[f];
        if (V.size() != 3)
        {
            std::cerr << "Attenzione: triangolazione classe I applicabile solo a triangoli.\n";
            continue;
        }

        unsigned int A = V[0], B = V[1], C = V[2];

        Eigen::Vector3d va = mesh.Cell0DsCoordinates.col(A);
        Eigen::Vector3d vb = mesh.Cell0DsCoordinates.col(B);
        Eigen::Vector3d vc = mesh.Cell0DsCoordinates.col(C);

        // Costruzione griglia di punti
        std::vector<std::vector<unsigned int>> pointGrid(b + 1);

        for (int i = 0; i <= b; ++i)
        {
            pointGrid[i].resize(b - i + 1);
            for (int j = 0; j <= b - i; ++j)
            {
                int k = b - i - j;

                // Interpolazione baricentrica
                Eigen::Vector3d p = (va * i + vb * j + vc * k) / static_cast<double>(b);
                p.normalize(); // proiezione sulla sfera

                // Inserisci vertice nella mesh finale (riusa se già esistente)
                newMesh.Cell0DsCoordinates.conservativeResize(3, currentVertexId + 1);
                newMesh.Cell0DsCoordinates.col(currentVertexId) = p;
                newMesh.Cell0DsId.push_back(currentVertexId);

                pointGrid[i][j] = currentVertexId;
                currentVertexId++;
                newMesh.NumCell0Ds++;
            }
        }

        // Costruzione dei sottotriangoli
        for (int i = 0; i < b; ++i)
        {
            for (int j = 0; j < b - i; ++j)
            {
                unsigned int v1 = pointGrid[i][j];
                unsigned int v2 = pointGrid[i + 1][j];
                unsigned int v3 = pointGrid[i][j + 1];
                AddCell2D(newMesh, {v1, v2, v3});

                if (j < b - i - 1)
                {
                    unsigned int v4 = pointGrid[i + 1][j + 1];
                    AddCell2D(newMesh, {v2, v4, v3});
                }
            }
        }
    }

    // Copia finale della mesh triangolata
    mesh = newMesh;
}

}
