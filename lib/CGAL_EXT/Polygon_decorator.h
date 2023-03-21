// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CGAL_EXT__Polygon_decorator__h__
#define __CGAL_EXT__Polygon_decorator__h__

#include <map>
#include <CGAL/HalfedgeDS_items_decorator.h>

namespace CGAL_EXT
{
  /**
   */
  template< class _HDS >
  class Polygon_decorator
    : public CGAL::HalfedgeDS_items_decorator< _HDS >
  {
  public:
    using HDS        = _HDS;
    using HalfedgeDS = _HDS;
    using Superclass = CGAL::HalfedgeDS_items_decorator< HDS >;
    using Self       = Polygon_decorator;

    using Traits   = typename HDS::Traits;
    using TNatural = typename Traits::TNatural;
    using Vertex   = typename HDS::Vertex;
    using Halfedge = typename HDS::Halfedge;
    using Face     = typename HDS::Face;

    using Vertex_handle         = typename HDS::Vertex_handle;
    using Vertex_const_handle   = typename HDS::Vertex_const_handle;
    using Vertex_iterator       = typename HDS::Vertex_iterator;
    using Vertex_const_iterator = typename HDS::Vertex_const_iterator;

    using Halfedge_handle         = typename HDS::Halfedge_handle;
    using Halfedge_const_handle   = typename HDS::Halfedge_const_handle;
    using Halfedge_iterator       = typename HDS::Halfedge_iterator;
    using Halfedge_const_iterator = typename HDS::Halfedge_const_iterator;

    using Face_handle         = typename HDS::Face_handle;
    using Face_const_handle   = typename HDS::Face_const_handle;
    using Face_iterator       = typename HDS::Face_iterator;
    using Face_const_iterator = typename HDS::Face_const_iterator;

    using size_type         = typename HDS::size_type;
    using difference_type   = typename HDS::difference_type;
    using iterator_category = typename HDS::iterator_category;

    using Supports_vertex_halfedge = typename HDS::Supports_vertex_halfedge;
    using Supports_halfedge_prev   = typename HDS::Supports_halfedge_prev;
    using Supports_halfedge_vertex = typename HDS::Supports_halfedge_vertex;
    using Supports_halfedge_face   = typename HDS::Supports_halfedge_face;
    using Supports_face_halfedge   = typename HDS::Supports_face_halfedge;
    using Supports_removal         = typename HDS::Supports_removal;

  protected:
    using _TCol   = std::map< TNatural, Halfedge_handle >;
    using _TEdges = std::map< TNatural, _TCol >;

  public:
    Polygon_decorator( HDS& ds );
    virtual ~Polygon_decorator( ) = default;

    template< class _PIt, class _TIt >
    void create_polygon( _PIt pb, _PIt pe, _TIt tb, _TIt te );

    template< class _PIt >
    void create_vertex_handles( _PIt b, _PIt e );

    Halfedge_handle create_border_edge( const TNatural& i, const TNatural& j );
    Halfedge_handle create_diagonal( const TNatural& i, const TNatural& j );
    Face_handle fill_hole( Halfedge_handle h );
    void triangulate( );

  protected:
    HDS* m_DS { nullptr };
    _TEdges m_Edges;
  };
} // end namespace

#include <CGAL_EXT/Polygon_decorator.hxx>

#endif // __CGAL_EXT__Polygon_decorator__h__

// eof - $RCSfile$
