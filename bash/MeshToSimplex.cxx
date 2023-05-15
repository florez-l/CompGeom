// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <CGAL/Cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/draw_polyhedron.h>

// -------------------------------------------------------------------------
// Types
using TKernel = CGAL::Cartesian< float >;
using TReal   = TKernel::FT;
using TPoint  = TKernel::Point_3;
using TMesh   = CGAL::Surface_mesh< TPoint >;
using TSimplex = CGAL::Polyhedron_3< TKernel >;

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  TMesh mesh;
  CGAL::IO::read_STL( argv[ 1 ], mesh );

  std::map< TMesh::Face_index, TKernel::Vector_3 > simplex_vertices;
  std::map< TMesh::Vertex_index, std::vector< TMesh::Face_index > > simplex_faces;

  for( const TMesh::Face_index& fIdx: mesh.faces( ) )
  {
    TKernel::Vector_3 fPnt( 0, 0, 0 );
    unsigned int fSize = 0;
    for(
      const TMesh::Vertex_index& vIdx:
        vertices_around_face( mesh.halfedge( fIdx ), mesh )
      )
    {
      fPnt += mesh.point( vIdx ) - CGAL::ORIGIN;
      fSize++;

      auto sfInsert =
        simplex_faces.insert(
          std::make_pair( vIdx, std::vector< TMesh::Face_index >( ) )
          );
      if( sfInsert.second )
      {
        TMesh::Halfedge_index hint = mesh.halfedge( vIdx );
        TMesh::Halfedge_index hIt = mesh.prev_around_source( hint );
        while( hIt != hint )
        {
          sfInsert.first->second.push_back( mesh.face( hIt ) );
          hIt = mesh.prev_around_source( hIt );
        } // end while
        sfInsert.first->second.push_back( mesh.face( hint ) );
      } // end if
    } // end for
    simplex_vertices.insert( std::make_pair( fIdx, fPnt / TReal( fSize ) ) );
  } // end for

  TSimplex simplex;
  CGAL::Polyhedron_incremental_builder_3< TSimplex::HalfedgeDS >
    builder( simplex.hds( ), true );
  builder.begin_surface( simplex_vertices.size( ), simplex_faces.size( ) );

  unsigned long long nVerts = 0;
  std::map< TMesh::Face_index, unsigned long long > f2v;
  for( const auto& sxVerts: simplex_vertices )
  {
    f2v[ sxVerts.first ] = nVerts++;
    builder.add_vertex(
      TSimplex::Point_3(
        sxVerts.second[ 0 ], sxVerts.second[ 1 ], sxVerts.second[ 2 ]
        )
      );
  } // end for
  std::cout << nVerts << " added vertices." << std::endl;

  std::map< unsigned long long, std::set< unsigned long long > > added_facets;
  unsigned long long nFaces = 0;
  for( const auto& sxFaces: simplex_faces )
  {
    // for( const TMesh::Face_index& fIdx: sxFaces.second )
    bool exists = false;
    for( unsigned long long i = 0; i < sxFaces.second.size( ); ++i )
    {
      auto iIt = added_facets.find( f2v[ sxFaces.second[ i ] ] );
      if( iIt != added_facets.end( ) )
      {
        auto jIt = iIt->second.find( f2v[ sxFaces.second[ ( i + 1 ) % sxFaces.second.size( ) ] ] );
        exists |= ( jIt != iIt->second.end( ) );
      } // end if
    } // end for

    if( !exists )
    {
      nFaces++;
      builder.begin_facet( );

      for( unsigned long long i = 0; i < sxFaces.second.size( ); ++i )
      {
        builder.add_vertex_to_facet( f2v[ sxFaces.second[ i ] ] );
        added_facets[ sxFaces.second[ i ] ].insert( sxFaces.second[ ( i + 1 ) % sxFaces.second.size( ) ] );
      } // end for
      builder.end_facet( );
    } // end if
  } // end for
  std::cout << nFaces << " added faces." << std::endl;

  builder.end_surface( );

  CGAL::draw( simplex );

  std::cerr << simplex << std::endl;



  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
