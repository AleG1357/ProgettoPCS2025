#include "PolyhedralMesh.hpp"
#include "PolyhedralMeshBuilder.hpp"
#include "ProjectionUtilities.hpp"
#include "UCDUtilities.hpp"
#include "SolidGenerator.hpp"

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace PolygonalLibrary;
using namespace Gedim;

int main()
{
    PolyhedralMesh mesh;

    // === 1. Genera il tetraedro regolare proiettato su sfera
    GenerateTetrahedron(mesh);

    // === 2. Esportazione in formato UCD .inp (compatibile con meshconverter.it)
    UCDUtilities exporter;

    // ==== 2.1 Cell0Ds.inp (vertici)
    {
        std::vector<UCDProperty<double>> cell0Ds_properties(1);
        cell0Ds_properties[0].Label = "Marker";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        std::vector<double> cell0Ds_marker(mesh.NumCell0Ds, 0.0);
        cell0Ds_properties[0].Data = cell0Ds_marker.data();

        exporter.ExportPoints("./Cell0Ds.inp",
                              mesh.Cell0DsCoordinates,
                              cell0Ds_properties);
    }

    // ==== 2.2 Cell1Ds.inp (lati)
    {
        std::vector<UCDProperty<double>> cell1Ds_properties(1);
        cell1Ds_properties[0].Label = "Marker";
        cell1Ds_properties[0].UnitLabel = "-";
        cell1Ds_properties[0].NumComponents = 1;

        std::vector<double> cell1Ds_marker(mesh.NumCell1Ds, 0.0);
        cell1Ds_properties[0].Data = cell1Ds_marker.data();

        exporter.ExportSegments("./Cell1Ds.inp",
                                mesh.Cell0DsCoordinates,
                                mesh.Cell1DsExtrema,
                                {},
                                cell1Ds_properties);
    }

    // ==== 2.3 Cell2Ds.inp (facce)
    {
        std::vector<UCDProperty<double>> cell2Ds_properties(1);
        cell2Ds_properties[0].Label = "Marker";
        cell2Ds_properties[0].UnitLabel = "-";
        cell2Ds_properties[0].NumComponents = 1;

        std::vector<double> cell2Ds_marker(mesh.NumCell2Ds, 0.0);
        cell2Ds_properties[0].Data = cell2Ds_marker.data();

        exporter.ExportPolygons("./Cell2Ds.inp",
                                mesh.Cell0DsCoordinates,
                                mesh.Cell2DsVertices,
                                {},
                                cell2Ds_properties);
    }

    cout << "Esportazione completata. Convertili su meshconverter.it e visualizzali con Glance." << endl;
    return 0;
}
