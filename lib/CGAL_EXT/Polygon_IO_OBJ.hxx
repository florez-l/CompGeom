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
  using _Point   = typename _P::container_type::value_type;
  using _Natural = typename _T::container_type::value_type;
  using _Kernel  = typename CGAL::Kernel_traits< _Point >::Kernel;
  using _Scalar  = typename _Kernel::FT;

  // Read file
  std::ifstream in_str( fname.c_str( ) );
  std::string line;
  std::getline( in_str, line );
  while( !in_str.eof( ) )
  {
    if( line[ 0 ] == 'v' )
    {
      std::istringstream v( line.substr( 1 ) );
      _Scalar x, y, z;
      v >> x >> y >> z;
      *geometry = _Point( x, y );
    }
    else if( line[ 0 ] == 'l' )
    {
      std::istringstream l( line.substr( 1 ) );
      unsigned long long i;
      l >> i;
      while( !l.eof( ) )
      {
        *topology = i - 1;
        l >> i;
      } // end while
    } // end if
    std::getline( in_str, line );
  } // end while
  in_str.close( );
}

// -------------------------------------------------------------------------
template< class _PIt, class _H >
void CGAL_EXT::
Polygon_saveOBJ(
  const std::string& fname, _PIt gBegin, _PIt gEnd, const _H& ds
  )
{
  std::ofstream out( fname.c_str( ) );
  out << "# Vertices" << std::endl;
  for( _PIt pIt = gBegin; pIt != gEnd; ++pIt )
    out << "v " << *pIt << " 0.0" << std::endl;

  out << std::endl << "# Faces" << std::endl;
  for( typename _H::Face_const_handle f: ds.face_handles( ) )
  {
    typename _H::Halfedge_const_handle i = f->halfedge( );
    out << "f " << i->vertex( )->point( ) + 1;
    i = i->next( );
    while( i != f->halfedge( ) )
    {
      out << " " << i->vertex( )->point( ) + 1;
      i = i->next( );
    } // end while
    out << std::endl;
  } // end for
  out << std::endl << "# eof" << std::endl;
  out.close( );
}

#endif // __CGAL_EXT__Polygon_readOBJ__hxx__

// eof - $RCSfile$
