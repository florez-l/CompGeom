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

/**
 */
template< class _K >
class PolygonTriangulation
{
public:
  struct Traits
  {
    using Point_2 = unsigned long;
  };
  using K = _K;
  using P = typename _K::Point_2;
  using DS = CGAL::HalfedgeDS_list< Traits, CGAL::HalfedgeDS_items_2 >;

public:
  PolygonTriangulation( )
    {
    }
  virtual ~PolygonTriangulation( )
    {
      if( this->m_DS != nullptr )
        delete this->m_DS;
    }

  void LoadFromOBJ( const std::string& fname )
    {
      // Prepare data
      std::vector< unsigned long long > polygon;
      this->m_Points.clear( );

      // Read file
      std::ifstream in_str( fname.c_str( ) );
      std::string line;
      std::getline( in_str, line );
      while( !in_str.eof( ) )
      {
        if( line[ 0 ] == 'v' )
        {
          std::istringstream v( line.substr( 1 ) );
          typename K::FT x, y, z;
          v >> x >> y >> z;
          this->m_Points.push_back(
            std::make_pair( P( x, y ), typename DS::Vertex_handle( ) )
            );
        }
        else if( line[ 0 ] == 'l' )
        {
          std::istringstream l( line.substr( 1 ) );
          while( !l.eof( ) )
          {
            unsigned long long i;
            l >> i;
            polygon.push_back( i - 1 );
          } // end while
        } // end if
        std::getline( in_str, line );
      } // end while
      in_str.close( );

      // Create halfedge data structure
      unsigned long n = this->m_Points.size( );
      if( this->m_DS != nullptr )
        delete this->m_DS;
      this->m_DS = new DS( n, ( n << 2 ) - 6, n - 2 );

      // Create vertices
      unsigned long long i = 0;
      for( auto& p: this->m_Points )
        p.second =
          this->m_DS->vertices_push_back( typename DS::Vertex( i++ ) );

      // Create edges
      auto eHnd = this->_create_segment( polygon[ 0 ], polygon[ 1 ] );
      for( i = 1; i < polygon.size( ); ++i )
        eHnd = this->_create_segment( polygon[ i ], eHnd );
    }

  void SaveToOBJ( const std::string& fname )
    {
      if( this->m_DS == nullptr )
        return;

      for( auto vIt = this->m_DS->vertices_begin( ); vIt != this->m_DS->vertices_end( ); ++vIt )
        std::cout << "v " << vIt->point( ) << " 0.0" << std::endl;

      std::cout << "faces: " << this->m_DS->size_of_faces( ) << std::endl;
      for( auto fIt = this->m_DS->faces_begin( ); fIt != this->m_DS->faces_end( ); ++fIt )
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

protected:
  typename DS::Halfedge_handle _create_segment(
    unsigned long long i,
    unsigned long long j
    )
    {
      typename DS::Halfedge_handle h =
        this->m_DS->edges_push_back(
          typename DS::Halfedge( ), typename DS::Halfedge( )
          );
      h->DS::Halfedge::Base::set_next( h->opposite( ) );
      h->opposite( )->DS::Halfedge::Base::set_next( h );
      h->set_prev( h->opposite( ) );
      h->opposite( )->set_prev( h );
      h->set_vertex( this->m_Points[ i ].second );
      h->opposite( )->set_vertex( this->m_Points[ j ].second );

      auto f = this->m_DS->faces_push_back( typename DS::Face( ) );

      h->set_face( f );
      h->opposite( )->set_face( f );
      f->DS::Face::Base::set_halfedge( h );

      this->m_Points[ i ].second->DS::Vertex::Base::set_halfedge( h );
      this->m_Points[ j ].second->
        DS::Vertex::Base::set_halfedge( h->opposite( ) );

      return( h );
    }

  typename DS::Halfedge_handle _create_segment(
    unsigned long long j,
    typename DS::Halfedge_handle e
    )
    {
      typename DS::Halfedge_handle h =
        this->m_DS->edges_push_back(
          typename DS::Halfedge( ), typename DS::Halfedge( )
          );
      h->DS::Halfedge::Base::set_next( h->opposite( ) );
      e->set_next( h );
      h->set_prev( e->opposite( ) );
      h->opposite( )->set_prev( h );
      h->set_vertex( e->opposite( )->vertex( ) );
      h->opposite( )->set_vertex( this->m_Points[ j ].second );

      h->set_face( e->face( ) );
      h->opposite( )->set_face( e->opposite( )->face( ) );

      this->m_Points[ j ].second->
        DS::Vertex::Base::set_halfedge( h->opposite( ) );

      return( h );
    }


protected:
  DS* m_DS { nullptr };
  std::vector< std::pair< P, typename DS::Vertex_handle > > m_Points;
};

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

  PolygonTriangulation< _K > tr;
  tr.LoadFromOBJ( argv[ 1 ] );
  tr.SaveToOBJ( "out.obj" );

  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
