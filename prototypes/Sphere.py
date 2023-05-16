## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

import itertools, math, numpy, random, sys, vtk

def create_points_in_sphere( ):
  ox = random.uniform( 50, 150 )
  oy = random.uniform( 50, 150 )
  oz = random.uniform( 50, 150 )
  r = random.uniform( 0.1, 100 )

  P = []
  for i in range( 4 ):
    u = random.uniform( 0, 4.0 * math.atan( 1 ) )
    v = random.uniform( 0, 8.0 * math.atan( 1 ) )

    x = r * math.sin( u ) * math.cos( v )
    y = r * math.sin( u ) * math.sin( v )
    z = r * math.cos( u )

    P += [ [ ox + x, oy + y, oz + z ] ]
  # end for

  return [ r, [ ox, oy, oz ], P ]
  
# end def

def normal( p1, p2, p3, p4 ):
  n  = numpy.cross( p1, p2 )[ 0 ]
  n += numpy.cross( p2, p3 )[ 0 ]
  n += numpy.cross( p3, p1 )[ 0 ]
  m = ( n @ n.T ) ** -0.5
  n *= m

  print( '--->', n @ ( p4 - p1 ).T )

  
  return n * m
# end def

def transformation_to_XY( p1, p2, p3, p4 ):
  n = normal( p1, p2, p3, p4 )

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

  T = ( p2 - p1 ) @ R.T
  Tx = T[ 0, 0 ]
  Ty = T[ 0, 1 ]

  x2 = ( ( Tx ** 2 ) + ( Ty ** 2 ) ) ** 0.5
  Rx = numpy.zeros( ( 3, 3 ) )
  Rx[ 0, 0 ] = Rx[ 1, 1 ] = Tx / x2
  Rx[ 0, 1 ] = Rx[ 1, 0 ] = Ty / x2
  Rx[ 1, 0 ] *= -1
  Rx[ 2, 2 ] = 1
  R2 = Rx @ R

  t3 = ( p3 - p1 ) @ R2.T
  t4 = ( p4 - p1 ) @ R2.T
  x3 = t3[ 0, 0 ]
  y3 = t3[ 0, 1 ]
  x4 = t4[ 0, 0 ]
  y4 = t4[ 0, 1 ]
  z4 = t4[ 0, 2 ]

  return [ n, R2, numpy.matrix( p1 ), [ x2, x3, y3, x4, y4, z4 ] ]
# end def

real_R, real_C, P = create_points_in_sphere( )

a = numpy.ones( ( 4, 4 ) )
a[ 0 , 0 : 3 ] = P[ 1 ]
a[ 1 , 0 : 3 ] = P[ 2 ]
a[ 2 , 0 : 3 ] = P[ 3 ]
a[ 3 , 0 : 3 ] = P[ 0 ]

Dx = numpy.ones( ( 4, 4 ) )
Dy = numpy.ones( ( 4, 4 ) )
Dz = numpy.ones( ( 4, 4 ) )

Dx[ 0, 0 ] = Dy[ 0, 0 ] = Dz[ 0, 0 ] = ( P[ 1 ] @ P[ 1 ].T )[ 0, 0 ]
Dx[ 1, 0 ] = Dy[ 1, 0 ] = Dz[ 1, 0 ] = ( P[ 2 ] @ P[ 2 ].T )[ 0, 0 ]
Dx[ 2, 0 ] = Dy[ 2, 0 ] = Dz[ 2, 0 ] = ( P[ 3 ] @ P[ 3 ].T )[ 0, 0 ]
Dx[ 3, 0 ] = Dy[ 3, 0 ] = Dz[ 3, 0 ] = ( P[ 0 ] @ P[ 0 ].T )[ 0, 0 ]

Dx[ 0, 1 ] = P[ 1 ][ 0, 1 ] ; Dx[ 0, 2 ] = P[ 1 ][ 0, 2 ]
Dx[ 1, 1 ] = P[ 2 ][ 0, 1 ] ; Dx[ 1, 2 ] = P[ 2 ][ 0, 2 ]
Dx[ 2, 1 ] = P[ 3 ][ 0, 1 ] ; Dx[ 2, 2 ] = P[ 3 ][ 0, 2 ]
Dx[ 3, 1 ] = P[ 0 ][ 0, 1 ] ; Dx[ 3, 2 ] = P[ 0 ][ 0, 2 ]

Dy[ 0, 1 ] = P[ 1 ][ 0, 0 ] ; Dy[ 0, 2 ] = P[ 1 ][ 0, 2 ]
Dy[ 1, 1 ] = P[ 2 ][ 0, 0 ] ; Dy[ 1, 2 ] = P[ 2 ][ 0, 2 ]
Dy[ 2, 1 ] = P[ 3 ][ 0, 0 ] ; Dy[ 2, 2 ] = P[ 3 ][ 0, 2 ]
Dy[ 3, 1 ] = P[ 0 ][ 0, 0 ] ; Dy[ 3, 2 ] = P[ 0 ][ 0, 2 ]

Dz[ 0, 1 ] = P[ 1 ][ 0, 0 ] ; Dz[ 0, 2 ] = P[ 1 ][ 0, 1 ]
Dz[ 1, 1 ] = P[ 2 ][ 0, 0 ] ; Dz[ 1, 2 ] = P[ 2 ][ 0, 1 ]
Dz[ 2, 1 ] = P[ 3 ][ 0, 0 ] ; Dz[ 2, 2 ] = P[ 3 ][ 0, 1 ]
Dz[ 3, 1 ] = P[ 0 ][ 0, 0 ] ; Dz[ 3, 2 ] = P[ 0 ][ 0, 1 ]

da = 2.0 * numpy.linalg.det( a )
dDx =  numpy.linalg.det( Dx )
dDy = -numpy.linalg.det( Dy )
dDz =  numpy.linalg.det( Dz )

print( '---------------------------------' )
print( dDx / da , dDy / da, dDz / da )
print( '---------------------------------' )


#n, Pr, Pt, [ x2, x3, y3, x4, y4, z4 ] = transformation_to_XY(
#  P[ 1 ][ 0 ], P[ 2 ][ 0 ], P[ 3 ][ 0 ], P[ 0 ][ 0 ]
#  )

# print( '1 -->', ( P[ 1 ][ 0 ] - Pt ) @ Pr.T )
# print( '2 -->', ( P[ 2 ][ 0 ] - Pt ) @ Pr.T, x2 )
# print( '3 -->', ( P[ 3 ][ 0 ] - Pt ) @ Pr.T, x3, y3 )
# print( '4 -->', ( P[ 0 ][ 0 ] - Pt ) @ Pr.T, x4, y4, z4 )
# print( P )

# Circumcircle and circumsphere
#cx = x2 * 0.5
#cy = ( ( x3 ** 2 ) + ( y3 ** 2 ) - ( x2 * x3 ) ) / ( 2.0 * y3 )
#Cz = ( ( y3 * z4 * z4 ) + ( y3 * ( ( x4 ** 2 ) + ( y4 ** 2 ) - ( x2 * x4 ) ) ) - ( y3 * y3 * y4 ) + ( y4 * ( ( x2 * x3 ) - ( x3 ** 2 ) ) ) ) / ( 2.0 * y3 * z4 )
# cc_y = cs_y = ( ( x2 * x3 ) - ( x3 ** 2 ) - ( y3 ** 2 ) ) / ( 2.0 * y3 )
# cs_y *= -1
# cs_z = ( ( y3 * ( ( x4 ** 2 ) + ( y4 ** 2 ) + ( z4 ** 2 ) - ( x2 * x4 ) ) ) - ( y4 * ( ( y3 ** 2 ) + ( x2 * x3 ) - ( x3 ** 2 ) ) ) ) / ( 2.0 * y3 * z4 )

#cc = numpy.matrix( [ cx, cy, 0 ] )
#sc = numpy.matrix( [ cx, -cy, Cz ] )

# print( 'circle --> ', cc, ( cc @ Pr ) + Pt )
# print( 'p1 --> ', ( ( ( cc @ Pr ) + Pt - P[ 1 ][ 0 ] ) @ ( ( cc @ Pr ) + Pt - P[ 1 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
# print( 'p2 --> ', ( ( ( cc @ Pr ) + Pt - P[ 2 ][ 0 ] ) @ ( ( cc @ Pr ) + Pt - P[ 2 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
# print( 'p3 --> ', ( ( ( cc @ Pr ) + Pt - P[ 3 ][ 0 ] ) @ ( ( cc @ Pr ) + Pt - P[ 3 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
# print( 'sphere --> ', sc, ( sc @ Pr ) + Pt )
# print( 'p1 --> ', ( ( ( sc @ Pr ) + Pt - P[ 1 ][ 0 ] ) @ ( ( sc @ Pr ) + Pt - P[ 1 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
# print( 'p2 --> ', ( ( ( sc @ Pr ) + Pt - P[ 2 ][ 0 ] ) @ ( ( sc @ Pr ) + Pt - P[ 2 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
# print( 'p3 --> ', ( ( ( sc @ Pr ) + Pt - P[ 3 ][ 0 ] ) @ ( ( sc @ Pr ) + Pt - P[ 3 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
# print( 'p4 --> ', ( ( ( sc @ Pr ) + Pt - P[ 0 ][ 0 ] ) @ ( ( sc @ Pr ) + Pt - P[ 0 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )

# print( '----------------------------------' ) 
# print( 'p1 --> ', ( sc @ sc.T )[ 0, 0 ] ** 0.5 )
# print( 'p2 --> ', ( ( sc - numpy.matrix( [ x2, 0, 0 ] ) ) @ ( sc - numpy.matrix( [ x2, 0, 0 ] ) ).T )[ 0, 0 ] ** 0.5 )
# print( 'p3 --> ', ( ( sc - numpy.matrix( [ x3, y3, 0 ] ) ) @ ( sc - numpy.matrix( [ x3, y3, 0 ] ) ).T )[ 0, 0 ] ** 0.5 )
# print( 'p4 --> ', ( ( sc - numpy.matrix( [ x4, y4, z4 ] ) ) @ ( sc - numpy.matrix( [ x4, y4, z4 ] ) ).T )[ 0, 0 ] ** 0.5 )

# # cc_C = numpy.matrix( [ cc_x, cc_y, 0 ] )
# # cs_C = numpy.matrix( [ cs_x, cs_y, cs_z ] )
# # cc_R = ( cc_C @ cc_C.T )[ 0, 0 ] ** 0.5
# # cs_R = ( cs_C @ cs_C.T )[ 0, 0 ] ** 0.5

# # c_phi = cs_z / cs_R
# # s_phi = ( cc_R / cs_R )
# # if z4 > 0:
# #   s_phi *= -1.0
# # # end if
# # H = s_phi / cc_R


print( '-------------------------------------------' )
print( 'Real radius:', real_R )
print( 'Real center:', real_C )
print( '-------------------------------------------' )
# print( 'Normal:', n )
# print( 'Circle center:', cc_C )
# print( 'Circle radius:', cc_R )
# print( 'Sphere center:', ( cs_C @ Pr ) + Pt )
# print( 'Sphere radius:', cs_R )
>>>>>>> ee7b5f02853ae4091343d667f2d7b3024177191f
# print( '-------------------------------------------' )
# print( 'Real radius:', real_R )
# print( 'Real center:', real_C )
# print( '-------------------------------------------' )
# # print( 'Normal:', n )
# # print( 'Circle center:', cc_C )
# # print( 'Circle radius:', cc_R )
# # print( 'Sphere center:', ( cs_C @ Pr ) + Pt )
# # print( 'Sphere radius:', cs_R )
# # print( '-------------------------------------------' )
# # print( 'Phi:', c_phi, s_phi )
# # print( 'H:', H )
# # print( '-------------------------------------------' )

## eof - $RCSfile$
