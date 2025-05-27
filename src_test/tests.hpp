#include "gtest/gtest.h"
#include "../src/PolyhedralMesh.hpp"
#include "../src/PolyhedralMeshBuilder.hpp"
#include "../src/GeodesicGenerator.hpp"
#include "../src/DualMeshGenerator.hpp"

using namespace PolygonalLibrary;

TEST(GeodesicGenerationTest, ClassI_ValidCounts)
{
    PolyhedralMesh mesh;
    GenerateGeodesicPolyhedronClassI(mesh, 3, 3, 2);

    RemoveDuplicateVertices(mesh);
    RemoveDuplicateEdges(mesh);

    int T = 2 * 2 + 2 * 0 + 0 * 0;
    EXPECT_EQ(mesh.NumCell0Ds, 2 * T + 2);  // V
    EXPECT_EQ(mesh.NumCell1Ds, 6 * T);      // E
    EXPECT_EQ(mesh.NumCell2Ds, 4 * T);      // F
}

TEST(MeshCleaningTest, RemovesDuplicateVertices)
{
    PolyhedralMesh mesh;
    // Costruisci mesh con 2 vertici identici
    AddCell0D(mesh, 0.0, 0.0, 0.0);
    AddCell0D(mesh, 0.0, 0.0, 0.0);  // duplicato
    EXPECT_EQ(mesh.NumCell0Ds, 2);

    RemoveDuplicateVertices(mesh);
    EXPECT_EQ(mesh.NumCell0Ds, 1);
}

TEST(MeshCleaningTest, RemovesDuplicateEdges)
{
    PolyhedralMesh mesh;
    AddCell0D(mesh, 0.0, 0.0, 0.0); // id 0
    AddCell0D(mesh, 1.0, 0.0, 0.0); // id 1
    AddCell1D(mesh, 0, 1);
    AddCell1D(mesh, 1, 0); // stesso lato, direzione inversa
    EXPECT_EQ(mesh.NumCell1Ds, 2);

    RemoveDuplicateEdges(mesh);
    EXPECT_EQ(mesh.NumCell1Ds, 1);
}
