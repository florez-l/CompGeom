## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

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

  # 3. Sort points
  U = [ ( t, P[ t ] ) for t in T ]
  print( U )
  
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
  for t in T:
    triangulate_monotone_polygon( P, t )
  # end for
# end if

## eof - $RCSfile$
