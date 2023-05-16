## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

# -- Base configuration
cmake_minimum_required(VERSION 3.0)
project(MyConvexHullAlgorithms VERSION 1.0.0.1)

# -- Output dirs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# -- Packages
find_package(CGAL REQUIRED COMPONENTS Qt5)

# -- Subdirs
subdirs(bash)

# -- Executables
#add_executable(monotone_partition_example_01 monotone_partition_example_01.cxx)
#target_link_libraries(monotone_partition_example_01 CGAL::CGAL)

#add_executable(monotone_partition_example_02 monotone_partition_example_02.cxx)
#target_link_libraries(monotone_partition_example_02 CGAL::CGAL)

#add_executable(manual_polygon manual_polygon.cxx)
#target_link_libraries(manual_polygon ${VTK_LIBRARIES})
#vtk_module_autoinit(TARGETS manual_polygon MODULES ${VTK_LIBRARIES})

## eof - CMakeLists.txt
