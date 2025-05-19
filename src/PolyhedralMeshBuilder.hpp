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

}
