#pragma once

#include <Eigen/Eigen>
#include <vector>
#include <map>

namespace PolygonalLibrary
{

struct PolyhedralMesh
{
    // === NUMERO DI ELEMENTI
    unsigned int NumCell0Ds = 0;
    unsigned int NumCell1Ds = 0;
    unsigned int NumCell2Ds = 0;
    unsigned int NumCell3Ds = 0;

    // === CELL0D (VERTICI)
    std::vector<unsigned int> Cell0DsId;                    // ID vertici
    Eigen::MatrixXd Cell0DsCoordinates;                     // 3 x NumCell0Ds
    std::map<unsigned int, std::vector<unsigned int>> MarkerCell0Ds;

    // === CELL1D (LATI)
    std::vector<unsigned int> Cell1DsId;                    // ID lati
    Eigen::MatrixXi Cell1DsExtrema;                         // 2 x NumCell1Ds
    std::map<unsigned int, std::vector<unsigned int>> MarkerCell1Ds;

    // === CELL2D (FACCE)
    std::vector<unsigned int> Cell2DsId;                    // ID facce
    std::vector<std::vector<unsigned int>> Cell2DsVertices; // lista vertici (ciclici) per ogni faccia
    std::vector<std::vector<unsigned int>> Cell2DsEdges;    // lista lati (ordinati) per ogni faccia
    std::map<unsigned int, std::vector<unsigned int>> MarkerCell2Ds;

    // === CELL3D (POLIEDRI)
    std::vector<unsigned int> Cell3DsId;                    // ID poliedri
    std::vector<std::vector<unsigned int>> Cell3DsVertices; // lista di vertici per ciascun poliedro
    std::vector<std::vector<unsigned int>> Cell3DsEdges;    // lista di lati
    std::vector<std::vector<unsigned int>> Cell3DsFaces;    // lista di facce
    std::map<unsigned int, std::vector<unsigned int>> MarkerCell3Ds;
};

}
