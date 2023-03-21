// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CGAL_EXT__Polygon_readOBJ__h__
#define __CGAL_EXT__Polygon_readOBJ__h__

#include <string>

namespace CGAL_EXT
{
  /**
   */
  template< class _P, class _T >
  void Polygon_readOBJ( const std::string& fname, _P geometry, _T topology );

} // end namespace

#include <CGAL_EXT/Polygon_readOBJ.hxx>

#endif // __CGAL_EXT__Polygon_readOBJ__h__

// eof - $RCSfile$
