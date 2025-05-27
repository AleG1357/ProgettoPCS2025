#pragma once

#include "PolyhedralMesh.hpp"
#include <cmath>
#include <vector>

namespace PolygonalLibrary
{

void AddCell0D(PolyhedralMesh& mesh, double x, double y, double z)
{
    unsigned int id = mesh.NumCell0Ds;

    mesh.Cell0DsId.resize(id + 1);
    mesh.Cell0DsId[id] = id;

    mesh.Cell0DsCoordinates.conservativeResize(3, id + 1);
    mesh.Cell0DsCoordinates(0, id) = x;
    mesh.Cell0DsCoordinates(1, id) = y;
    mesh.Cell0DsCoordinates(2, id) = z;

    mesh.NumCell0Ds = id + 1;
}

void AddCell1D(PolyhedralMesh& mesh, unsigned int origin, unsigned int end)
{
    unsigned int id = mesh.NumCell1Ds;

    mesh.Cell1DsId.resize(id + 1);
    mesh.Cell1DsId[id] = id;

    mesh.Cell1DsExtrema.conservativeResize(2, id + 1);
    mesh.Cell1DsExtrema(0, id) = origin;
    mesh.Cell1DsExtrema(1, id) = end;

    mesh.NumCell1Ds = id + 1;
}

void AddCell2D(PolyhedralMesh& mesh, const std::vector<unsigned int>& vertexIDs)
{
    unsigned int id = mesh.NumCell2Ds;
    unsigned int N = vertexIDs.size();

    mesh.Cell2DsId.resize(id + 1);
    mesh.Cell2DsId[id] = id;

    mesh.Cell2DsVertices.resize(id + 1);
    mesh.Cell2DsVertices[id].resize(N);

    mesh.Cell2DsEdges.resize(id + 1);
    mesh.Cell2DsEdges[id].resize(N);

    for (unsigned int i = 0; i < N; ++i)
    {
        mesh.Cell2DsVertices[id][i] = vertexIDs[i];
        unsigned int origin = vertexIDs[i];
        unsigned int end = vertexIDs[(i + 1) % N];

        AddCell1D(mesh, origin, end);
        mesh.Cell2DsEdges[id][i] = mesh.NumCell1Ds - 1;
    }

    mesh.NumCell2Ds = id + 1;
}


void RemoveDuplicateVertices(PolyhedralMesh& mesh, double epsilon = 1e-6)
{
    std::vector<int> oldToNew(mesh.NumCell0Ds, -1);
    std::vector<Eigen::Vector3d> newCoords;
    std::vector<unsigned int> newIds;

    for (unsigned int i = 0; i < mesh.NumCell0Ds; ++i)
    {
        Eigen::Vector3d pi = mesh.Cell0DsCoordinates.col(i);
        bool found = false;
        for (unsigned int j = 0; j < newCoords.size(); ++j)
        {
            if ((pi - newCoords[j]).norm() < epsilon)
            {
                oldToNew[i] = j;
                found = true;
                break;
            }
        }
        if (!found)
        {
            oldToNew[i] = newCoords.size();
            newCoords.push_back(pi);
            newIds.push_back(mesh.Cell0DsId[i]);
        }
    }

    mesh.Cell0DsCoordinates.resize(3, newCoords.size());
    for (unsigned int i = 0; i < newCoords.size(); ++i)
        mesh.Cell0DsCoordinates.col(i) = newCoords[i];

    mesh.Cell0DsId = newIds;
    mesh.NumCell0Ds = newCoords.size();

    for (int i = 0; i < mesh.Cell1DsExtrema.cols(); ++i)
    {
        mesh.Cell1DsExtrema(0, i) = oldToNew[mesh.Cell1DsExtrema(0, i)];
        mesh.Cell1DsExtrema(1, i) = oldToNew[mesh.Cell1DsExtrema(1, i)];
    }

    for (auto& verts : mesh.Cell2DsVertices)
    {
        for (auto& v : verts)
            v = oldToNew[v];
    }
}



void RemoveDuplicateEdges(PolyhedralMesh& mesh)
{
    std::vector<unsigned int> newIds;
    std::vector<Eigen::Vector2i> newEdges;
    std::map<std::pair<int, int>, int> edgeMap;

    for (int i = 0; i < mesh.Cell1DsExtrema.cols(); ++i)
    {
        int a = mesh.Cell1DsExtrema(0, i);
        int b = mesh.Cell1DsExtrema(1, i);
        if (a > b) std::swap(a, b);

        auto key = std::make_pair(a, b);
        if (edgeMap.find(key) == edgeMap.end())
        {
            edgeMap[key] = static_cast<int>(newEdges.size());
            newIds.push_back(mesh.Cell1DsId[i]);
            newEdges.emplace_back(a, b);
        }
    }

    mesh.Cell1DsExtrema.resize(2, newEdges.size());

    for (unsigned int i = 0; i < newEdges.size(); ++i)
    {
        mesh.Cell1DsExtrema(0, i) = newEdges[i][0];  // ✅ PRIMO vertice (origin)
        mesh.Cell1DsExtrema(1, i) = newEdges[i][1];  // ✅ SECONDO vertice (end)
    }

    mesh.Cell1DsId = newIds;
    mesh.NumCell1Ds = static_cast<int>(newEdges.size());
}

}