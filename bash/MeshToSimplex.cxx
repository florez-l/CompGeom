// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

#include <CGAL/Cartesian.h>
#include <CGAL/Kernel_traits.h>
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
template< class _M >
typename CGAL::Kernel_traits< typename _M::Point >::Kernel::Vector_3
face_normal( const _M& m, const typename _M::Face_index& f )
{
  using _K = typename CGAL::Kernel_traits< typename _M::Point >::Kernel;

  typename _M::Halfedge_index h = m.halfedge( f );
  typename _K::Vector_3 p1 = m.point( m.source( h ) ) - CGAL::ORIGIN;
  typename _K::Vector_3 p2 = m.point( m.source( m.next( h ) ) ) - CGAL::ORIGIN;
  typename _K::Vector_3 p3 = m.point( m.target( m.next( h ) ) ) - CGAL::ORIGIN;
  typename _K::Vector_3 n;

  n  = CGAL::cross_product( p1, p2 );
  n += CGAL::cross_product( p2, p3 );
  n += CGAL::cross_product( p3, p1 );
  n /= std::sqrt( n.squared_length( ) );
  return( n );
}

// -------------------------------------------------------------------------
template< class _M >
typename CGAL::Kernel_traits< typename _M::Point >::Kernel::Point_3
face_center( const _M& m, const typename _M::Face_index& f )
{
  using _K = typename CGAL::Kernel_traits< typename _M::Point >::Kernel;
  using _P = typename _K::Point_3;

  typename _M::Halfedge_index h = m.halfedge( f );
  typename _K::Vector_3 p;
  p  = m.point( m.source( h ) ) - CGAL::ORIGIN;
  p += m.point( m.source( m.next( h ) ) ) - CGAL::ORIGIN;
  p += m.point( m.target( m.next( h ) ) ) - CGAL::ORIGIN;

  return( _P( p[ 0 ] / 3.0, p[ 1 ] / 3.0, p[ 2 ] / 3.0 ) );
}

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  TMesh mesh;
  CGAL::IO::read_STL( argv[ 1 ], mesh );

  TSimplex simplex;
  CGAL::Polyhedron_incremental_builder_3< TSimplex::HalfedgeDS >
    builder( simplex.hds( ), true );
  builder.begin_surface(
    mesh.faces( ).size( ),
    mesh.vertices( ).size( )
    );

  std::map< TMesh::Face_index, TSimplex::Vertex_handle > simplex_vertices;

  std::set< TMesh::Face_index > marks;
  std::queue< TMesh::Face_index > qf;
  qf.push( *( mesh.faces( ).begin( ) ) );
  while( qf.size( ) > 0 )
  {
    TMesh::Face_index f = qf.front( );
    qf.pop( );

    if( marks.find( f ) == marks.end( ) )
    {
      marks.insert( f );

      simplex_vertices[ f ] = builder.add_vertex( face_center( mesh, f ) );

      TMesh::Halfedge_index h = mesh.halfedge( f );
      q.push( mesh.face( mesh.opposite( h ) ) );
      q.push( mesh.face( mesh.opposite( mesh.next( h ) ) ) );
      q.push( mesh.face( mesh.opposite( mesh.prev( h ) ) ) );
    } // end if
  } // end while

  builder.end_surface( );

  /* TODO

  

  */

  // TODO: TMesh::Face_index& fIdx: mesh.faces( )


  /* TODO
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
  */

  std::cerr << simplex << std::endl;


  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
