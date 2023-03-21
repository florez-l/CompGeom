// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>

#include <CGAL/Cartesian.h>
#include <CGAL/HalfedgeDS_items_2.h>
#include <CGAL/HalfedgeDS_list.h>

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
  std::vector< TPoint > points;
  points.push_back( TPoint( 10,  0 ) );
  points.push_back( TPoint(  0,  0 ) );
  points.push_back( TPoint( 10, 10 ) );
  points.push_back( TPoint(  0, 10 ) );
  TTraits::TNatural polygon[] = { 3, 1, 0, 2 };

  TDS ds( 4, 10, 2 );
  TPolygon_decorator triang( ds );

  triang.create_polygon(
    points.begin( ), points.end( ),
    polygon, polygon + 4
    );
  triang.create_diagonal( 3, 0 );

  // Save results
  CGAL_EXT::Polygon_saveOBJ(
    "square_triangles.obj", points.begin( ), points.end( ), ds
    );

  // Show a final report
  std::cout << "Vertices: " << ds.size_of_vertices( ) << std::endl;
  std::cout << "Edges   : " << ds.size_of_halfedges( ) << std::endl;
  std::cout << "Faces   : " << ds.size_of_faces( ) << std::endl;

  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
