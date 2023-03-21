## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

import functools
import Polygon

'''
'''
def triangulate_monotone_polygon( P, T ):

  # 1. Check orientation
  a = Polygon.area( P, T )
  ccwT = T.copy( )
  if a < float( 0 ):
    ccwT.reverse( )
  # end if

  # 2. Is the input polygon a triangle?

  # 3. Sort polygon points
  def point_comparison( a, b ):
    if a[ 1 ][ 1 ] == b[ 1 ][ 1 ]:
      return a[ 1 ][ 0 ] - b[ 1 ][ 0 ]
    else:
      return b[ 1 ][ 1 ] - a[ 1 ][ 1 ]
    # end if
  # end def
  U = \
    sorted( \
      [ ( i, P[ ccwT[ i ] ] ) for i in range( len( ccwT ) ) ],
      key = functools.cmp_to_key( point_comparison ) \
    )

  # 4. Mark chains
  C = Polygon.chains( P, U[ 0 ][ 0 ], U[ -1 ][ 0 ], ccwT )

  # 5. Initialize output
  D = []
  for i in range( len( ccwT ) ):
    D += [ [ ccwT[ i ], ccwT[ ( i + 1 ) % len( ccwT ) ] ] ]
  # end for

  # 6. Main loop
  S = [ U[ 0 ], U[ 1 ] ]
  for j in range( 2, len( U ) - 1 ):
    if C[ U[ j ][ 0 ] ] != C[ S[ -1 ][ 0 ] ]:
      while len( S ) > 1:
        pv = S[ -1 ]
        S.pop( )
        D += [ [ ccwT[ U[ j ][ 0 ] ], ccwT[ pv[ 0 ] ] ] ]
        pv = S[ -1 ]
      # end while
      S = [ U[ j - 1 ], U[ j ] ]
    else:
      S.pop( )
      stop = False
      pv = None
      while not stop and len( S ) > 0:
        if Polygon.edge_inside( P, U[ j ][ 0 ], S[ -1 ][ 0 ], ccwT ):
          pv = S[ -1 ]
          D += [ [ ccwT[ U[ j ][ 0 ] ], ccwT[ S[ -1 ][ 0 ] ] ] ]
          S.pop( )
        else:
          stop = True
        # end if
      # end while
      if not pv is None:
        S += [ pv ]
      # end if
      S += [ U[ j ] ]
    # end if
  # end for

  # 7. Remaining diagonals
  if len( S ) > 2:
    S.pop( )
    while len( S ) > 1:
      D += [ [ ccwT[ U[ n ][ 0 ] ], ccwT[ S[ -1 ][ 0 ] ] ] ]
      S.pop( )
    # end while
  # end if

  return D
# end def

## -------------------------------------------------------------------------
if __name__ == '__main__':
  import sys
  if len( sys.argv ) < 3:
    print( 'Usage: ' + sys.argv[ 0 ] + ' input.obj output.obj' )
    sys.exit( 1 )
  # end if

  # Read data
  P, T = Polygon.load_polygons_from_OBJ( sys.argv[ 1 ] )
  D = []
  for t in T:
    D += triangulate_monotone_polygon( P, t )
  # end for
  Polygon.save_polygons_as_OBJ( sys.argv[ 2 ], P, D )
# end if


## eof - $RCSfile$






















