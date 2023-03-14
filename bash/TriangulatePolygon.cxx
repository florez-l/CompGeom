// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <list>
#include <map>
#include <string>

#include <CGAL/Cartesian.h>
#include <CGAL/HalfedgeDS_items_2.h>
#include <CGAL/HalfedgeDS_decorator.h>
#include <CGAL/HalfedgeDS_list.h>

#include <CGAL/partition_2.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/property_map.h>

// -------------------------------------------------------------------------
template< class _PntIt, class _PolyIt >
void ReadOBJ( const std::string& fname, _PntIt points, _PolyIt polygon );

template< class _Pnts, class _Poly >
void triangulate( const _Pnts& points, const _Poly& polygon )
{
  using _P = typename _Pnts::value_type;
  using _K = typename CGAL::Kernel_traits< _P >::Kernel;
  using _M = typename CGAL::Pointer_property_map< _P >::type;
  using _T = CGAL::Partition_traits_2< _K, _M >;
  struct _TrTraits { using Point_2 = unsigned long; };
  using _TrDS = CGAL::HalfedgeDS_list< _TrTraits, CGAL::HalfedgeDS_items_2 >;
  using _Tr = CGAL::HalfedgeDS_decorator< _TrDS >;

  // Prepare output
  unsigned long n = points.size( );
  _TrDS trds( n, ( n << 2 ) - 6, n - 2 );
  _Tr tr( trds );

  // Add initial segments
  typename _Tr::Halfedge_handle eHnd, fHnd;
  for(
    auto pIt = polygon.container( ).begin( );
    pIt != polygon.container( ).end( );
    ++pIt
    )
  {
    if( pIt == polygon.container( ).begin( ) )
    {
      eHnd = tr.create_segment( );
      eHnd->vertex( )->point( ) = *pIt;
      fHnd = eHnd;
    } // end if
    else
    {
      eHnd->opposite( )->vertex( )->point( ) = *pIt;
      typename _TrDS::Halfedge_handle h =
        trds.edges_push_back( typename _TrDS::Halfedge( ), typename _TrDS::Halfedge( ) );
      h->_TrDS::Halfedge::Base::set_next( h->opposite( ) );
      // h->opposite( )->_TrDS::Halfedge::Base::set_next( h );
      eHnd->set_next( h );
      tr.set_prev( h, eHnd->opposite( ) );
      tr.set_prev( h->opposite( ), h );
      tr.set_vertex( h, eHnd->opposite( )->vertex( ) );
      tr.set_vertex( h->opposite( ), tr.vertices_push_back( typename _Tr::Vertex( ) ) );
      tr.set_face( h, tr.get_face( eHnd ) );
      tr.set_face( h->opposite( ), tr.get_face( h ) );
      // tr.set_face_halfedge( h );
      tr.set_vertex_halfedge( h->opposite( ) );
      eHnd = h;

    } // end if
  } // end for

  /* TODO
     eHnd->opposite( )->vertex( )->point( ) = fHnd->vertex( )->point( );
     tr.insert_halfedge( eHnd->opposite( ), fHnd->opposite( ) );
     // tr.join_vertex( eHnd->opposite( ) );
     std::cout << "--> " << fHnd->vertex( )->point( ) << std::endl;
  */

  /* TODO
     tr.make_hole( fHnd );
  */



  // Partition into monotone polygons
  /* TODO
     _T traits( CGAL::make_property_map( *( const_cast< _Pnts* >( &points ) ) ) );
     std::list< typename _T::Polygon_2 > partition;
     CGAL::y_monotone_partition_2(
     polygon.vertices_begin( ), polygon.vertices_end( ),
     std::back_inserter( partition ), traits
     );
  */

  // Add initial edges
  /* TODO
     std::map< unsigned long, std::map< unsigned long, typename _Tr::Halfedge_handle > > edges_hnd;
     for( const auto& poly: partition )
     {
     for( auto pIt = poly.container( ).begin( ); pIt != poly.container( ).end( ); ++pIt )
     {
     auto qIt = pIt;
     qIt++;
     bool end = ( qIt == poly.container( ).end( ) );
     if( end )
     qIt = poly.container( ).begin( );
      
     bool found = false;
     auto eIt = edges_hnd.find( *pIt );
     if( eIt != edges_hnd.end( ) )
     found = eIt->second.find( *qIt ) != eIt->second.end( );
     if( !found )
     {
     _TrE e, d;
     e.set_vertex( points_hnd[ *pIt ] );
     d.set_vertex( points_hnd[ *qIt ] );
     edges_hnd[ *pIt ][ *qIt ] = tr.edges_push_back( e, d );
     edges_hnd[ *qIt ][ *pIt ] = edges_hnd[ *pIt ][ *qIt ];

     if( end )
     {
     auto eIt = edges_hnd[ *pIt ][ *qIt ];
     
     typename _Tr::Face f;
     f.set_halfedge( eIt );
     auto hnd = tr.faces_push_back( f );
     
     eIt->set_face( hnd );
     eIt = eIt->next( );
     while( eIt != edges_hnd[ *pIt ][ *qIt ] )
     {
     eIt->set_face( hnd );
     eIt = eIt->next( );
     }
     } // end if
     // TODO: } // end if
     } // end for
     } // end for
     
     t.edges_push_back( e );
     
     _Tr t;
     std::cout << t << std::endl;
  */
     
  for( auto vIt = trds.vertices_begin( ); vIt != trds.vertices_end( ); ++vIt )
    std::cout << "v " << vIt->point( ) << " 0.0" << std::endl;
  
  std::cout << "faces: " << trds.size_of_faces( ) << std::endl;
  for( auto fIt = trds.faces_begin( ); fIt != trds.faces_end( ); ++fIt )
  {
    auto hIt = fIt->halfedge( );
    std::cout << hIt->vertex( )->point( ) << " " << hIt->opposite( )->vertex( )->point( ) << std::endl;
    hIt = hIt->next( );
    while( hIt != fIt->halfedge( ) )
    {
      std::cout << hIt->vertex( )->point( ) << " " << hIt->opposite( )->vertex( )->point( ) << std::endl;
      hIt = hIt->next( );
    }
    std::cout << "==========" << std::endl;
  }
}

// -------------------------------------------------------------------------
// Types
using _K = CGAL::Cartesian< double >;
using _M = CGAL::Pointer_property_map< _K::Point_2 >::type;
using _T = CGAL::Partition_traits_2< _K, _M >;

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

  // Read input polygon
  std::vector< _K::Point_2 > points;
  _T::Polygon_2 polygon;
  ReadOBJ(
    argv[ 1 ],
    std::back_inserter( points ),
    std::back_inserter( polygon.container( ) )
    );
  polygon.container( ).pop_back( );

  // Triangulate polygon
  triangulate( points, polygon );

  return( EXIT_SUCCESS );
}

// -------------------------------------------------------------------------
template< class _PntIt, class _PolyIt >
void ReadOBJ( const std::string& fname, _PntIt points, _PolyIt polygon )
{
  using _P = typename _PntIt::container_type::value_type;
  using _K = typename CGAL::Kernel_traits< _P >::Kernel;
  using _S = typename _K::FT;

  std::ifstream in_str( fname.c_str( ) );
  std::string line;
  std::getline( in_str, line );
  while( !in_str.eof( ) )
  {
    if( line[ 0 ] == 'v' )
    {
      std::istringstream v( line.substr( 1 ) );
      _S x, y, z;
      v >> x >> y >> z;
      *points = _P( x, y );
    }
    else if( line[ 0 ] == 'l' )
    {
      std::istringstream l( line.substr( 1 ) );
      while( !l.eof( ) )
      {
        unsigned long long i;
        l >> i;
        *polygon = i - 1;
      } // end while
    } // end if
    std::getline( in_str, line );
  } // end while
  in_str.close( );
}

// eof - $RCSfile$
