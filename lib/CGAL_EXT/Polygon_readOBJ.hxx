// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CGAL_EXT__Polygon_readOBJ__hxx__
#define __CGAL_EXT__Polygon_readOBJ__hxx__

#include <fstream>

// -------------------------------------------------------------------------
template< class _P, class _T >
void CGAL_EXT::
Polygon_readOBJ( const std::string& fname, _P geometry, _T topology )
{
  // Prepare data
  /* TODO
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
  */
}

#endif // __CGAL_EXT__Polygon_readOBJ__hxx__

// eof - $RCSfile$
