// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CGAL_EXT__Polygon_IO_OBJ__h__
#define __CGAL_EXT__Polygon_IO_OBJ__h__

#include <string>

namespace CGAL_EXT
{
  /**
   */
  template< class _P, class _T >
  void Polygon_readOBJ( const std::string& fname, _P geometry, _T topology );

  /**
   */
  template< class _PIt, class _H >
  void Polygon_saveOBJ(
    const std::string& fname,
    _PIt gBegin, _PIt gEnd,
    const _H& ds
    );

} // end namespace

#include <CGAL_EXT/Polygon_IO_OBJ.hxx>

#endif // __CGAL_EXT__Polygon_IO_OBJ__h__

// eof - $RCSfile$
