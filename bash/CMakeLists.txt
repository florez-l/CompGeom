
## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

include_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(CompGeom_TriangulateSquare TriangulateSquare.cxx)
target_link_libraries(CompGeom_TriangulateSquare CGAL::CGAL)

add_executable(CompGeom_TriangulatePolygon TriangulatePolygon.cxx)
target_link_libraries(CompGeom_TriangulatePolygon CGAL::CGAL)

add_executable(CompGeom_MeshSphere MeshSphere.cxx)
target_link_libraries(CompGeom_MeshSphere CGAL::CGAL)

add_executable(CompGeom_MeshToSimplex MeshToSimplex.cxx)
target_link_libraries(
  CompGeom_MeshToSimplex
  CGAL::CGAL
  CGAL::CGAL_Basic_viewer
  )

## eof - CMakeLists.txt
