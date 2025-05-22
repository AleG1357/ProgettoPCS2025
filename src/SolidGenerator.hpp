#pragma once

#include "PolyhedralMesh.hpp"
#include "PolyhedralMeshBuilder.hpp"
#include "ProjectionUtilities.hpp"
#include <vector>
#include <cmath>
#include <iostream>

namespace PolygonalLibrary
{

inline void GenerateTetrahedron(PolyhedralMesh& mesh)
{
    mesh = PolyhedralMesh();

    std::vector<std::vector<double>> coords = {
        { 1,  1,  1},
        {-1, -1,  1},
        {-1,  1, -1},
        { 1, -1, -1}
    };

    for (const auto& v : coords)
        AddCell0D(mesh, v[0], v[1], v[2]);

    std::vector<std::vector<unsigned int>> faces = {
        {0, 1, 2},
        {0, 3, 1},
        {0, 2, 3},
        {1, 3, 2}
    };

    for (const auto& face : faces)
        AddCell2D(mesh, face);

    ProjectVerticesOnUnitSphere(mesh);
}

inline void GenerateCube(PolyhedralMesh& mesh)
{
    mesh = PolyhedralMesh();

    std::vector<std::vector<double>> coords = {
        {-1, -1, -1}, { 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1},
        {-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, {-1,  1,  1}
    };

    for (const auto& v : coords)
        AddCell0D(mesh, v[0], v[1], v[2]);

    std::vector<std::vector<unsigned int>> faces = {
        {0, 1, 2, 3}, {4, 5, 6, 7}, {0, 1, 5, 4},
        {2, 3, 7, 6}, {1, 2, 6, 5}, {3, 0, 4, 7}
    };

    for (const auto& face : faces)
        AddCell2D(mesh, face);

    ProjectVerticesOnUnitSphere(mesh);
}

inline void GenerateOctahedron(PolyhedralMesh& mesh)
{
    mesh = PolyhedralMesh();

    std::vector<std::vector<double>> coords = {
        { 1, 0, 0}, {-1, 0, 0}, {0, 1, 0},
        {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
    };

    for (const auto& v : coords)
        AddCell0D(mesh, v[0], v[1], v[2]);

    std::vector<std::vector<unsigned int>> faces = {
        {0, 2, 4}, {2, 1, 4}, {1, 3, 4}, {3, 0, 4},
        {2, 0, 5}, {1, 2, 5}, {3, 1, 5}, {0, 3, 5}
    };

    for (const auto& face : faces)
        AddCell2D(mesh, face);

    ProjectVerticesOnUnitSphere(mesh);
}

inline void GenerateIcosahedron(PolyhedralMesh& mesh)
{
    mesh = PolyhedralMesh();

    const double phi = (1.0 + std::sqrt(5.0)) / 2.0;

    std::vector<std::vector<double>> coords = {
        {-1,  phi,  0}, { 1,  phi,  0}, {-1, -phi,  0}, { 1, -phi,  0},
        { 0, -1,  phi}, { 0,  1,  phi}, { 0, -1, -phi}, { 0,  1, -phi},
        { phi,  0, -1}, { phi,  0,  1}, {-phi,  0, -1}, {-phi,  0,  1}
    };

    for (const auto& v : coords)
        AddCell0D(mesh, v[0], v[1], v[2]);

    std::vector<std::vector<unsigned int>> faces = {
        {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7,10}, {0,10,11},
        {1, 5, 9}, {5,11, 4}, {11,10,2}, {10, 7, 6}, {7, 1, 8},
        {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
        {4, 9, 5}, {2, 4,11}, {6, 2,10}, {8, 6, 7}, {9, 8, 1}
    };

    for (const auto& face : faces)
        AddCell2D(mesh, face);

    ProjectVerticesOnUnitSphere(mesh);
}

// Dodecaedro = duale dell'icosaedro → per ora non implementato

inline void DetectAndGeneratePlatonic(PolyhedralMesh& mesh, int p, int q)
{
    if (p == 3 && q == 3)
        GenerateTetrahedron(mesh);
    else if (p == 3 && q == 4)
        GenerateOctahedron(mesh);
    else if (p == 4 && q == 3)
        GenerateCube(mesh);
    else if (p == 3 && q == 5)
        GenerateIcosahedron(mesh);
    else {
        std::cerr << "Coppia (p=" << p << ", q=" << q << ") non valida per un solido platonico." << std::endl;
    }
}

}
