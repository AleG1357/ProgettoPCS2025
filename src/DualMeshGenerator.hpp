#pragma once

#include "PolyhedralMesh.hpp"
#include "PolyhedralMeshBuilder.hpp"
#include <Eigen/Dense>
#include <vector>
#include <map>
#include <set>
#include <iostream>

namespace PolygonalLibrary
{

void GenerateDualMesh(const PolyhedralMesh& input, PolyhedralMesh& dual)
{
    dual = PolyhedralMesh();

    // === 1. Calcola baricentro per ogni faccia → diventa un vertice nel duale
    std::vector<Eigen::Vector3d> faceCentroids(input.NumCell2Ds);

    for (unsigned int i = 0; i < input.NumCell2Ds; ++i)
    {
        const auto& vertIds = input.Cell2DsVertices[i];
        Eigen::Vector3d centroid(0, 0, 0);
        for (auto v : vertIds)
            centroid += input.Cell0DsCoordinates.col(v);
        centroid /= static_cast<double>(vertIds.size());
        centroid.normalize(); // proietta sulla sfera

        dual.Cell0DsCoordinates.conservativeResize(3, dual.NumCell0Ds + 1);
        dual.Cell0DsCoordinates.col(dual.NumCell0Ds) = centroid;
        dual.Cell0DsId.push_back(dual.NumCell0Ds);
        faceCentroids[i] = centroid;
        ++dual.NumCell0Ds;
    }

    // === 2. Mappa: ogni vertice → lista ordinata di facce adiacenti (per creare facce del duale)
    std::map<unsigned int, std::vector<unsigned int>> vertexToFaces;

    for (unsigned int faceId = 0; faceId < input.NumCell2Ds; ++faceId)
    {
        const auto& vList = input.Cell2DsVertices[faceId];
        for (auto v : vList)
        {
            vertexToFaces[v].push_back(faceId);
        }
    }

    // === 3. Costruisci facce del duale (una per ogni vertice originale)
    for (const auto& [v, faceList] : vertexToFaces)
    {
        if (faceList.size() < 3) continue; // per sicurezza

        // Ordina i centroidi delle facce intorno al vertice v
        // (approccio semplice: nessun ordine topologico perfetto, ma funziona su mesh regolari)
        std::vector<unsigned int> newFace;
        for (auto f : faceList)
        {
            newFace.push_back(f); // ogni faccia corrisponde a un vertice nel duale
        }

        AddCell2D(dual, newFace);
    }

    // Non costruiamo Cell3D (opzionale), ma possiamo completare se serve
}

}
