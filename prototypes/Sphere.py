## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

import math, numpy, random

def create_points_in_sphere( ):
  ox = random.uniform( -100, 100 )
  oy = random.uniform( -100, 100 )
  oz = random.uniform( -100, 100 )
  r = random.uniform( 0.1, 100 )

  P = []
  for i in range( 4 ):
    u = random.uniform( 0, 4.0 * math.atan( 1 ) )
    v = random.uniform( 0, 8.0 * math.atan( 1 ) )

    x = r * math.sin( u ) * math.cos( v )
    y = r * math.sin( u ) * math.sin( v )
    z = r * math.cos( u )

    P += [ numpy.matrix( [ ox + x, oy + y, oz + z ] ) ]
  # end for

  return [ r, numpy.matrix( [ ox, oy, oz ] ), P ]
  
# end def

def normal( p1, p2, p3 ):
  n  = numpy.cross( p1, p2 )[ 0 ]
  n += numpy.cross( p2, p3 )[ 0 ]
  n += numpy.cross( p3, p1 )[ 0 ]
  m = ( n @ n.T ) ** -0.5
  return n * m
# end def

def transformation_to_XY( p1, p2, p3, p4 ):
  n = normal( p1, p2, p3 )

  ct = n[ 2 ]
  st = ( ( n[ 0 ] ** 2 ) + ( n[ 1 ] ** 2 ) ) ** 0.5
  u1 = n[ 1 ] / st
  u2 = -n[ 0 ] / st

  R = numpy.zeros( ( 3, 3 ) )
  R[ 0, 0 ] = ct + ( u1 * u1 * ( 1.0 - ct ) )
  R[ 1, 1 ] = ct + ( u2 * u2 * ( 1.0 - ct ) )
  R[ 2, 2 ] = ct
  R[ 0, 1 ] = u1 * u2 * ( 1.0 - ct )
  R[ 1, 0 ] = u1 * u2 * ( 1.0 - ct )
  R[ 0, 2 ] = u2 * st
  R[ 2, 0 ] = -u2 * st
  R[ 1, 2 ] = -u1 * st
  R[ 2, 1 ] = u1 * st

  T = ( ( R @ ( p2 - p1 ).T ).T )[ 0 ]
  Tx = T[ 0, 0 ]
  Ty = T[ 0, 1 ]

  x2 = ( ( Tx ** 2 ) + ( Ty ** 2 ) ) ** 0.5
  Rx = numpy.zeros( ( 3, 3 ) )
  Rx[ 0, 0 ] = Rx[ 1, 1 ] = Tx / x2
  Rx[ 0, 1 ] = Rx[ 1, 0 ] = Ty / x2
  Rx[ 1, 0 ] *= -1
  Rx[ 2, 2 ] = 1
  R = Rx @ R

  t3 = ( p3 - p1 ) @ R.T
  t4 = ( p4 - p1 ) @ R.T
  x3 = t3[ 0, 0 ]
  y3 = t3[ 0, 1 ]
  x4 = t4[ 0, 0 ]
  y4 = t4[ 0, 1 ]
  z4 = t4[ 0, 2 ]

  return [ n, R, p1, [ x2, x3, y3, x4, y4, z4 ] ]
# end def

real_R, real_C, P = create_points_in_sphere( )

n, Pr, Pt, [ x2, x3, y3, x4, y4, z4 ] = transformation_to_XY(
  P[ 1 ][ 0 ], P[ 2 ][ 0 ], P[ 3 ][ 0 ], P[ 0 ][ 0 ]
  )

# Circumcircle center
cc_C = ( Pr @ numpy.matrix( [ x2, ( ( x3 ** 2 ) + ( y3 ** 2 ) - ( x2 * x3 ) ) / y3, 0.0 ] ).T ).T + Pt

print( '-------------------------------------------' )
print( 'Real radius:', real_R )
print( 'Real center:', real_C )
print( '-------------------------------------------' )
print( 'Normal:', n )
print( 'Circumcenter:', cc_C )
print( '-------------------------------------------' )


# o = ( R.T @ numpy.matrix( [ x2 / 2, ( ( x3 ** 2 ) + ( y3 ** 2 ) - ( x2 * x3 ) ) / ( 2 * y3 ) , 0.0 ] ).T ).T + P[ 1 ]
# r  = ( ( o - P[ 1 ] ) @ ( o - P[ 1 ] ).T )[ 0, 0 ] ** 0.5
# r += ( ( o - P[ 2 ] ) @ ( o - P[ 2 ] ).T )[ 0, 0 ] ** 0.5
# r += ( ( o - P[ 3 ] ) @ ( o - P[ 3 ] ).T )[ 0, 0 ] ** 0.5
# r /= 3.0

# Ma = numpy.ones( ( 4, 4 ) )
# Ma[ 0, 0 ] = P[ 0 ][ 0, 0 ]
# Ma[ 0, 1 ] = P[ 0 ][ 0, 1 ]
# Ma[ 0, 2 ] = P[ 0 ][ 0, 2 ]
# Ma[ 1, 0 ] = P[ 1 ][ 0, 0 ]
# Ma[ 1, 1 ] = P[ 1 ][ 0, 1 ]
# Ma[ 1, 2 ] = P[ 1 ][ 0, 2 ]
# Ma[ 2, 0 ] = P[ 2 ][ 0, 0 ]
# Ma[ 2, 1 ] = P[ 2 ][ 0, 1 ]
# Ma[ 2, 2 ] = P[ 2 ][ 0, 2 ]
# Ma[ 3, 0 ] = P[ 3 ][ 0, 0 ]
# Ma[ 3, 1 ] = P[ 3 ][ 0, 1 ]
# Ma[ 3, 2 ] = P[ 3 ][ 0, 2 ]
# a = numpy.linalg.det( Ma )

# MD = numpy.ones( ( 4, 4 ) )
# MD[ 0, 0 ] = ( P[ 0 ] @ P[ 0 ].T )[ 0, 0 ]
# MD[ 1, 0 ] = ( P[ 1 ] @ P[ 1 ].T )[ 0, 0 ]
# MD[ 2, 0 ] = ( P[ 2 ] @ P[ 2 ].T )[ 0, 0 ]
# MD[ 3, 0 ] = ( P[ 3 ] @ P[ 3 ].T )[ 0, 0 ]
# MD[ 0, 1 ] = P[ 0 ][ 0, 1 ]
# MD[ 0, 2 ] = P[ 0 ][ 0, 2 ]
# MD[ 1, 1 ] = P[ 1 ][ 0, 1 ]
# MD[ 1, 2 ] = P[ 1 ][ 0, 2 ]
# MD[ 2, 1 ] = P[ 2 ][ 0, 1 ]
# MD[ 2, 2 ] = P[ 2 ][ 0, 2 ]
# MD[ 3, 1 ] = P[ 3 ][ 0, 1 ]
# MD[ 3, 2 ] = P[ 3 ][ 0, 2 ]
# Ox = numpy.linalg.det( MD ) / ( 2.0 * a )

# MD[ 0, 1 ] = P[ 0 ][ 0, 0 ]
# MD[ 0, 2 ] = P[ 0 ][ 0, 2 ]
# MD[ 1, 1 ] = P[ 1 ][ 0, 0 ]
# MD[ 1, 2 ] = P[ 1 ][ 0, 2 ]
# MD[ 2, 1 ] = P[ 2 ][ 0, 0 ]
# MD[ 2, 2 ] = P[ 2 ][ 0, 2 ]
# MD[ 3, 1 ] = P[ 3 ][ 0, 0 ]
# MD[ 3, 2 ] = P[ 3 ][ 0, 2 ]
# Oy = numpy.linalg.det( MD ) / ( -2.0 * a )

# MD[ 0, 1 ] = P[ 0 ][ 0, 0 ]
# MD[ 0, 2 ] = P[ 0 ][ 0, 1 ]
# MD[ 1, 1 ] = P[ 1 ][ 0, 0 ]
# MD[ 1, 2 ] = P[ 1 ][ 0, 1 ]
# MD[ 2, 1 ] = P[ 2 ][ 0, 0 ]
# MD[ 2, 2 ] = P[ 2 ][ 0, 1 ]
# MD[ 3, 1 ] = P[ 3 ][ 0, 0 ]
# MD[ 3, 2 ] = P[ 3 ][ 0, 1 ]
# Oz = numpy.linalg.det( MD ) / ( 2.0 * a )

# O = numpy.matrix( [ Ox, Oy, Oz ] )
# R  = ( ( O - P[ 0 ] ) @ ( O - P[ 0 ] ).T )[ 0, 0 ] ** 0.5
# R += ( ( O - P[ 1 ] ) @ ( O - P[ 1 ] ).T )[ 0, 0 ] ** 0.5
# R += ( ( O - P[ 2 ] ) @ ( O - P[ 2 ] ).T )[ 0, 0 ] ** 0.5
# R += ( ( O - P[ 3 ] ) @ ( O - P[ 3 ] ).T )[ 0, 0 ] ** 0.5
# R /= 4.0

# print( 'Normal:', n )
# print( 'Circle:', o, r )
# print( 'Sphere:', O, R )

# s_phi = ( r / R )
# if ( ( P[ 1 ] - P[ 0 ] ) @ n.T )[ 0, 0 ] < 0:
#   s_phi *= -1.0
# # end if

# o_O = o - O
# c_phi = ( ( o_O @ o_O.T )[ 0, 0 ] ** 0.5 ) / R
# if ( o_O @ n.T )[ 0, 0 ] < 0:
#   c_phi *= -1.0
# # end if

# print( 'Phi:', c_phi, s_phi, ( c_phi ** 2 ) + ( s_phi ** 2 ) )
# print( 'H:', s_phi / r )

## eof - $RCSfile$
