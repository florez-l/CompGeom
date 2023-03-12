## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

'''
Compute the cross product between two 3D vectors
'''
def cross( x, y ):
  return \
    ( \
      ( x[ 1 ] * y[ 2 ] ) - ( x[ 2 ] * y[ 1 ] ), \
      ( x[ 2 ] * y[ 0 ] ) - ( x[ 0 ] * y[ 2 ] ), \
      ( x[ 0 ] * y[ 1 ] ) - ( x[ 1 ] * y[ 0 ] ) \
    )
# end def

'''
Compute the side ("left", "right" or "over") where a 2D point p lies
according to a 2D line segment s.
'''
def side( s, p ):
  z = \
    cross( \
      ( \
        float( s[ 1 ][ 0 ] - s[ 0 ][ 0 ] ), \
        float( s[ 1 ][ 1 ] - s[ 0 ][ 1 ] ), \
        float( 0 )  \
      ), \
      ( float( p[ 0 ] ), float( p[ 1 ] ), float( 0 ) ) \
    )[ 2 ]
  if z > float( 0 ):
    return 'right'
  elif z < float( 0 ):
    return 'left'
  else:
    return 'over'
  # end if
# end def

## eof - $RCSfile$
