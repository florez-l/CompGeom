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
  n = normal( P[ 1 ][ 0 ], P[ 2 ][ 0 ], P[ 3 ][ 0 ] )

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

  p3 = R @ ( p3 - p1 ).T
  x3 = p3[ 0, 0 ]
  y3 = p3[ 1, 0 ]

  p4 = R @ ( p4 - p1 ).T
  x4 = p4[ 0, 0 ]
  y4 = p4[ 1, 0 ]
  z4 = p4[ 2, 0 ]

  return [ n, R, p1, [ x2, x3, y3, x4, y4, z4 ] ]
# end def

real_R, real_C, P = create_points_in_sphere( )
n, Pr, Pt, tr_xyz = transformation_to_XY(
  P[ 1 ][ 0 ], P[ 2 ][ 0 ], P[ 3 ][ 0 ], P[ 0 ][ 0 ]
  )
x2, x3, y3, x4, y4, z4 = tr_xyz

# Circumcircle and circumsphere
cc_x = cs_x = x2 * 0.5
cc_y = cs_y = ( ( x2 * x3 ) - ( x3 ** 2 ) - ( y3 ** 2 ) ) / ( 2.0 * y3 )
cs_z = ( ( x2 * ( ( x3 * y4 ) - ( x4 * y3 ) ) ) - ( ( y4 * ( ( x3 ** 2 ) + ( y3 ** 2 ) ) ) - ( y3 * ( ( x4 ** 2 ) + ( y4 ** 2 ) + ( z4 ** 2 ) ) ) ) ) / ( 2.0 * y3 * z4 )

cc_C = numpy.matrix( [ cc_x, cc_y, 0 ] )
cs_C = numpy.matrix( [ cs_x, cs_y, cs_z ] )
cc_R = ( cc_C @ cc_C.T )[ 0, 0 ] ** 0.5
cs_R = ( cs_C @ cs_C.T )[ 0, 0 ] ** 0.5

c_phi = cs_z / cs_R
s_phi = ( cc_R / cs_R )
if z4 > 0:
  s_phi *= -1.0
# end if
H = s_phi / cc_R


print( '-------------------------------------------' )
print( 'Real radius:', real_R )
print( 'Real center:', real_C )
print( '-------------------------------------------' )
print( 'Normal:', n )
print( 'Circle center:', cc_C )
print( 'Circle radius:', cc_R )
print( 'Sphere center:', ( Pr.T @ cs_C.T ).T + Pt )
print( 'Sphere radius:', cs_R )
print( '-------------------------------------------' )
print( 'Phi:', c_phi, s_phi )
print( 'H:', H )
print( '-------------------------------------------' )

## eof - $RCSfile$
