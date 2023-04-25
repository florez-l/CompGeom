// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>

#include <CGAL/Cartesian.h>
#include <CGAL/Surface_mesh.h>

#include <CGAL_EXT/Sphere_function.h>

// -------------------------------------------------------------------------
// Types
using TKernel = CGAL::Cartesian< double >;
using TReal = TKernel::FT;
using TPoint = TKernel::Point_3;
using TMesh = CGAL::Surface_mesh< TPoint >;
using TFunction = CGAL_EXT::Sphere_function< TKernel >;

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  // Properties
  TReal r = 3;
  unsigned int nU = 5;
  unsigned int nV = 5;
  TReal minV = 0;
  TReal maxV = TReal( 4 ) * TReal( std::atan( double( 1 ) ) );
  TReal minU = 0;
  TReal maxU = maxV * TReal( 2 );
  bool closedU = true;
  bool closedV = false;

  // Base objects
  TMesh m;
  TFunction f;
  f.set_radius( r );

  // Geometry
  for( unsigned int iU = 0; iU < nU; ++iU )
  {
    TReal u = TReal( iU ) / TReal( nU - ( ( closedU )? 0: 1 ) );
    u = ( u * ( maxU - minU ) ) + minU;
    for( unsigned int iV = 0; iV < nV; ++iV )
    {
      TReal v = TReal( iV ) / TReal( nV - ( ( closedV )? 0: 1 ) );
      v = ( v * ( maxV - minV ) ) + minV;

      m.add_vertex( f.evaluate( u, v ) );
    } // end for
  } // end for

  // Topology

  // Save results
  CGAL::IO::write_OBJ( "mesh.obj", m );
  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
