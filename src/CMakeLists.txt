# message("SUBFOLDER:")


list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/PolyhedralMesh.hpp)
list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/PolyhedralMeshBuilder.hpp)
list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/ProjectionUtilities.hpp)
list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/SolidGenerator.hpp)
list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/GeodesicGenerator.hpp)
list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/GeodesicSubdivision.hpp)



#list(APPEND src_sources ${CMAKE_CURRENT_SOURCE_DIR}/Pippo.cpp)

list(APPEND src_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(src_headers ${src_headers} PARENT_SCOPE)
set(src_sources ${src_sources} PARENT_SCOPE)
set(src_includes ${src_includes} PARENT_SCOPE)
