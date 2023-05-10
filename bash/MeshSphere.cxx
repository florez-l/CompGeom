// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>

#include <CGAL/Cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

#include <CGAL_EXT/Sphere_function.h>

// -------------------------------------------------------------------------
// Types
// using TKernel = CGAL::Cartesian< float >;
using TKernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using TReal = TKernel::FT;
using TPoint = TKernel::Point_3;
using TMesh = CGAL::Surface_mesh< TPoint >;
using TFunction = CGAL_EXT::Sphere_function< TKernel >;

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  struct SPointCmp
  {
    bool operator()( const TPoint& a, const TPoint& b )
      {
        auto d = CGAL::squared_distance( a, b );
        if( d <= decltype( d )( 1e-2 ) )
          return( false );
        return( a < b );
      }
  };

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
  std::vector< std::vector< TMesh::Vertex_index > > T( nU, std::vector< TMesh::Vertex_index >( nV ) );
  std::map< TPoint, TMesh::Vertex_index, SPointCmp > M; 
  for( unsigned int iU = 0; iU < nU; ++iU )
  {
    TReal u = TReal( iU ) / TReal( nU - ( ( closedU )? 0: 1 ) );
    u = ( u * ( maxU - minU ) ) + minU;
    for( unsigned int iV = 0; iV < nV; ++iV )
    {
      TReal v = TReal( iV ) / TReal( nV - ( ( closedV )? 0: 1 ) );
      v = ( v * ( maxV - minV ) ) + minV;

      TPoint p = f.evaluate( u, v );
      auto mIt = M.find( p );
      if( mIt == M.end( ) )
        mIt = M.insert( std::make_pair( p, m.add_vertex( p ) ) ).first;
      T[ iU ][ iV ] = mIt->second;
    } // end for
  } // end for

  std::cout << nU * nV << " " << M.size( ) << std::endl;

  // Topology
  for( unsigned int iV = 0; iV < nV - 1; ++iV )
  {
    for( unsigned int iU = 0; iU < nU - 1; ++iU )
    {
      m.add_face( T[ iU ][ iV ], T[ iU + 1 ][ iV ], T[ iU ][ iV + 1 ] );
      m.add_face( T[ iU + 1 ][ iV ], T[ iU + 1 ][ iV + 1 ], T[ iU ][ iV + 1 ] );
    } // end for
  } // end for

  // Save results
  CGAL::IO::write_OBJ( "mesh.obj", m );
  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
