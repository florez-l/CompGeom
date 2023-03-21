// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CGAL_EXT__Polygon_decorator__hxx__
#define __CGAL_EXT__Polygon_decorator__hxx__

#include <iterator>
#include <stdexcept>

// -------------------------------------------------------------------------
template< class _HDS >
CGAL_EXT::Polygon_decorator< _HDS >::
Polygon_decorator( HDS& ds )
  : m_DS( &ds )
{
}

// -------------------------------------------------------------------------
template< class _HDS >
template< class _PIt, class _TIt >
void CGAL_EXT::Polygon_decorator< _HDS >::
create_polygon( _PIt pb, _PIt pe, _TIt tb, _TIt te )
{
  // Geometry
  this->create_vertex_handles( pb, pe );

  // Topology
  _TIt i, j;
  i = j = tb;
  for( j++; j != te; ++i, ++j )
    this->create_border_edge( *i, *j );
  this->fill_hole( this->create_border_edge( *i, *tb ) );
}

// -------------------------------------------------------------------------
template< class _HDS >
template< class _PIt >
void CGAL_EXT::Polygon_decorator< _HDS >::
create_vertex_handles( _PIt b, _PIt e )
{
  TNatural i = 0;
  for( _PIt p = b; p != e; ++p )
    this->m_DS->vertices_push_back( Vertex( i++ ) );
}

// -------------------------------------------------------------------------
template< class _HDS >
typename CGAL_EXT::Polygon_decorator< _HDS >::
Halfedge_handle CGAL_EXT::Polygon_decorator< _HDS >::
create_border_edge( const TNatural& i, const TNatural& j )
{
  TNatural nV = this->m_DS->size_of_vertices( );
  if( i >= nV || j >= nV )
    throw std::out_of_range( "Vertices not yet added." );

  typename _TEdges::const_iterator rIt;
  typename _TCol::const_iterator cIt;

  // Try to find if the edge already exists
  bool found = false;
  rIt = this->m_Edges.find( i );
  if( rIt != this->m_Edges.end( ) )
  {
    cIt = rIt->second.find( j );
    found = ( cIt != rIt->second.end( ) );
  } // end if
  if( found )
    return( cIt->second );

  // Not found, then create a new one
  Halfedge_handle h = this->m_DS->edges_push_back( Halfedge( ), Halfedge( ) );
  h->set_next( h );
  h->set_prev( h );
  h->opposite( )->set_next( h->opposite( ) );
  h->opposite( )->set_prev( h->opposite( ) );

  // Get vertices
  Vertex_iterator iIt, jIt;
  iIt = jIt = this->m_DS->vertices_begin( );
  std::advance( iIt, i );
  std::advance( jIt, j );

  // Associate them
  h->set_vertex( iIt );
  h->opposite( )->set_vertex( jIt );

  // Link with previously created edges
  Halfedge_handle iHnd = iIt->halfedge( );
  Halfedge_handle jHnd = jIt->halfedge( );
  if( iHnd != Halfedge_handle( ) )
  {
    h->set_prev( iHnd->opposite( ) );
    h->opposite( )->set_next( iHnd );
    iHnd->set_prev( h->opposite( ) );
    iHnd->opposite( )->set_next( h );
    h->set_face( iHnd->opposite( )->face( ) );
    h->opposite( )->set_face( iHnd->face( ) );
  }
  else
    iIt->set_halfedge( h );

  if( jHnd != Halfedge_handle( ) )
  {
    h->set_next( jHnd );
    jHnd->set_prev( h );
    jHnd->opposite( )->set_next( h->opposite( ) );
    h->opposite( )->set_prev( jHnd->opposite( ) );
    h->set_face( jHnd->face( ) );
    h->opposite( )->set_face( jHnd->opposite( )->face( ) );
  }
  else
    jIt->set_halfedge( h->opposite( ) );

  // Keep track and return
  this->m_Edges[ i ][ j ] = h;
  this->m_Edges[ j ][ i ] = h->opposite( );
  return( h );
}

// -------------------------------------------------------------------------
template< class _HDS >
typename CGAL_EXT::Polygon_decorator< _HDS >::
Halfedge_handle CGAL_EXT::Polygon_decorator< _HDS >::
create_diagonal( const TNatural& i, const TNatural& j )
{
  TNatural nV = this->m_DS->size_of_vertices( );
  if( i >= nV || j >= nV )
    throw std::out_of_range( "Vertices not yet added." );

  typename _TEdges::const_iterator iIt, jIt;
  typename _TCol::const_iterator cIt, kIt, lIt;

  // Try to find if the edge already exists
  bool found = false;
  iIt = this->m_Edges.find( i );
  if( iIt != this->m_Edges.end( ) )
  {
    cIt = iIt->second.find( j );
    found = ( cIt != iIt->second.end( ) );
  } // end if
  if( found )
    return( cIt->second );

  // Find some coherent edges
  Halfedge_handle e, g;
  jIt = this->m_Edges.find( j );
  for( kIt = iIt->second.begin( ); kIt != iIt->second.end( ); ++kIt )
  {
    for( lIt = jIt->second.begin( ); lIt != jIt->second.end( ); ++lIt )
    {
      if( kIt->second != lIt->second )
      {
        if( kIt->second->face( ) != Face_handle( ) )
        {
          if( kIt->second->face( ) == lIt->second->face( ) )
          {
            e = kIt->second;
            g = lIt->second;
          } // end if
        } // end if
      } // ed if
    } // end for
  } // end for
  if( e == Halfedge_handle( ) || g == Halfedge_handle( ) )
    throw std::domain_error( "Both edges do not belong to the same face." );

  // Create new edge
  Halfedge_handle h = this->m_DS->edges_push_back( Halfedge( ), Halfedge( ) );

  // Assing vertices
  h->set_vertex( e->vertex( ) );
  h->opposite( )->set_vertex( g->vertex( ) );

  // Link it
  Halfedge_handle a = e->prev( );
  Halfedge_handle b = g->prev( );

  a->set_next( h );
  b->set_next( h->opposite( ) );

  h->set_next( g );
  h->set_prev( a );
  h->opposite( )->set_next( e );
  h->opposite( )->set_prev( b );

  // Old face
  Face_handle f1 = e->face( );
  f1->set_halfedge( e );
  h->opposite( )->set_face( f1 );

  // New face
  Face f2v;
  f2v.set_halfedge( h );
  Face_handle f2 = this->m_DS->faces_push_back( f2v );
  Halfedge_handle it = h;
  it->set_face( f2 );
  it = it->next( );
  while( it != h )
  {
    it->set_face( f2 );
    it = it->next( );
  } // end while

  // Keep track and return
  this->m_Edges[ i ][ j ] = h;
  this->m_Edges[ j ][ i ] = h->opposite( );
  return( h );
}

// -------------------------------------------------------------------------
template< class _HDS >
typename CGAL_EXT::Polygon_decorator< _HDS >::
Face_handle CGAL_EXT::Polygon_decorator< _HDS >::
fill_hole( Halfedge_handle h )
{
  Face_handle f = h->face( );
  if( f == Face_handle( ) )
  {
    Face fv;
    fv.set_halfedge( h );
    f = this->m_DS->faces_push_back( fv );

    Halfedge_handle i = h;
    i->set_face( f );
    i = i->next( );
    while( i != h )
    {
      i->set_face( f );
      i = i->next( );
    } // end while
  } // end if
  return( f );
}

// -------------------------------------------------------------------------
template< class _HDS >
void CGAL_EXT::Polygon_decorator< _HDS >::
triangulate( )
{
}

#endif // __CGAL_EXT__Polygon_decorator__hxx__

// eof - $RCSfile$
