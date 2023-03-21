// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <string>

#include <CGAL/Cartesian.h>
#include <CGAL/HalfedgeDS_items_2.h>
#include <CGAL/HalfedgeDS_list.h>
#include <CGAL/partition_2.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/property_map.h>

#include <CGAL_EXT/Polygon_decorator.h>
#include <CGAL_EXT/Polygon_IO_OBJ.h>

// -------------------------------------------------------------------------
// Types
using TReal = double;
struct TTraits
{
  using TNatural = unsigned long long;
  using Point_2 = TNatural;
};
using TKernel = CGAL::Cartesian< TReal >;
using TPoint = TKernel::Point_2;
using TDS = CGAL::HalfedgeDS_list< TTraits, CGAL::HalfedgeDS_items_2 >;
using TPolygon_decorator = CGAL_EXT::Polygon_decorator< TDS >;

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  if( argc < 3 )
  {
    std::cerr
      << "Usage: " << argv[ 0 ] << " input.obj output.obj"
      << std::endl;
    return( EXIT_FAILURE );
  } // end if

  // Read input
  std::vector< TPoint > points;
  std::vector< TTraits::TNatural > polygon;
  CGAL_EXT::Polygon_readOBJ(
    argv[ 1 ],
    std::back_inserter( points ),
    std::back_inserter( polygon )
    );

  // Create base polygon
  TTraits::TNatural n = polygon.size( );
  TDS ds( n, ( n << 2 ) - 6, n - 2 );
  TPolygon_decorator triang( ds );
  triang.create_polygon(
    points.begin( ), points.end( ),
    polygon.begin( ), polygon.end( )
    );
  triang.triangulate( );

  // Save results
  CGAL_EXT::Polygon_saveOBJ( argv[ 2 ], points.begin( ), points.end( ), ds );

  // Show a final report
  std::cout << "Vertices: " << ds.size_of_vertices( ) << std::endl;
  std::cout << "Edges   : " << ds.size_of_halfedges( ) << std::endl;
  std::cout << "Faces   : " << ds.size_of_faces( ) << std::endl;

  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
