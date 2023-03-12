## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

'''
Compute the area of a 2D polygon represented by a sequence of points P
and a sequence of vertices T
'''
def area( P, T = None ):
  if T is None:
    return area( P, [ i for i in range( len( P ) ) ] )
  else:
    a = float( 0 )
    for i in range( len( T ) ):
      j = ( i + 1 ) % len( T )
      a += ( float( P[ T[ i ] ][ 0 ] ) * float( P[ T[ j ] ][ 1 ] ) )
      a -= ( float( P[ T[ i ] ][ 1 ] ) * float( P[ T[ j ] ][ 0 ] ) )
    # end for
    return a * 0.5
  # end if
# end def

'''
Compute the chains of a 2D polygon
'''
def chains( P, s, e, T ):
  chains = [ 'positive' for i in range( len( T ) ) ]
  i = s
  while i != e:
    chains[ T[ i ] ] = 'negative'
    i = ( i + 1 ) % len( T )
  # end while
  return chains
# end def

'''
Compute the chains of a 2D polygon
'''
def chains( P, s, e ):
  return chains( P, s, e, [ i for i in range( len( P ) ) ] )
# end def

'''
'''
def load_polygons_from_OBJ( fname ):
  f = open( fname, 'r' )
  lines = f.readlines( )
  f.close( )
  P, T = [], []
  for line in lines:
    tokens = line.split( )
    if len( tokens ) > 1:
      if tokens[ 0 ] == 'v':
        P += [ [ float( x ) for x in tokens[ 1 : ] ] ]
      elif tokens[ 0 ] == 'l':
        T += [ [ int( i ) - 1 for i in tokens[ 1 : -1 ] ] ]
      # end if
    # end if
  # end for
  return ( P, T )
# end def

'''
'''
def save_polygons_as_OBJ( fname, P, T ):
  s = '# Points\n'
  for p in P:
    s += 'v'
    for x in p:
      s += ' ' + str( x )
    # end for
    s += '\n'
  # end for
  s = '\n# Indices\n'
  for t in T:
    s += 'l'
    for i in t:
      s += ' ' + str( i + 1 )
    # end for
    s += ' ' + str( t[ 0 ] + 1 )
    s += '\n'
  # end for
  f = open( fname, 'w' )
  f.write( s )
  f.close( )
# end def

'''
'''
def save_polygon_as_OBJ( fname, P ):
  save_polygons_as_OBJ( fname, P, [ [ i for i in range( len( P ) ) ] ] )
# end def

## eof - $RCSfile$
