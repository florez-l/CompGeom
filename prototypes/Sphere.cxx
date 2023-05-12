// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Dense>

int main( int argc, char** argv )
{
  using TReal = double;
  using TMatrix = Eigen::Matrix< TReal, 3, 3 >;
  using TCol = Eigen::Matrix< TReal, 3, 1 >;
  using TRow = Eigen::Matrix< TReal, 1, 3 >;

  std::random_device rd;
  std::mt19937 gen( rd( ) );
  std::uniform_real_distribution< TReal > coord_dis( -100, 100 );
  std::uniform_real_distribution< TReal > radius_dis( 0.1, 100 );
  std::uniform_real_distribution< TReal > u_dis( 0, 4 * std::atan( 1 ) );
  std::uniform_real_distribution< TReal > v_dis( 0, 8 * std::atan( 1 ) );
  TCol real_C;
  real_C << coord_dis( gen ), coord_dis( gen ), coord_dis( gen );
  TReal real_R = radius_dis( gen );

  std::vector< TCol > P( 4 );
  for( unsigned int i = 0; i < 4; ++i )
  {
    TReal u = u_dis( gen );
    TReal v = v_dis( gen );

    P[ i ]( 0 ) = real_R * std::sin( u ) * std::cos( v );
    P[ i ]( 1 ) = real_R * std::sin( u ) * std::sin( v );
    P[ i ]( 2 ) = real_R * std::cos( u );
  } // end for

  TCol n = P[ 1 ].cross( P[ 2 ] );
  n += P[ 2 ].cross( P[ 3 ] );
  n += P[ 3 ].cross( P[ 1 ] );
  n.array( ) /= ( n.transpose( ) * n ).array( ).sqrt( ).sum( );

  TReal ct = n( 2 );
  TReal st = std::sqrt( ( n( 0 ) * n( 0 ) ) + ( n( 1 ) * n( 1 ) ) );
  TReal u1 = n( 1 ) / st;
  TReal u2 = -n( 0 ) / st;

  TMatrix R;
  R( 0, 0 ) = ct + ( u1 * u1 * ( 1.0 - ct ) );
  R( 1, 1 ) = ct + ( u2 * u2 * ( 1.0 - ct ) );
  R( 2, 2 ) = ct;
  R( 0, 1 ) = u1 * u2 * ( 1.0 - ct );
  R( 1, 0 ) = u1 * u2 * ( 1.0 - ct );
  R( 0, 2 ) = u2 * st;
  R( 2, 0 ) = -u2 * st;
  R( 1, 2 ) = -u1 * st;
  R( 2, 1 ) = u1 * st;


  TCol t2 = R * ( P[ 2 ] - P[ 1 ] );
  TReal x2 = ( t2.transpose( ) * t2 ).array( ).sqrt( ).sum( );

  TMatrix Rxy;
  Rxy( 0, 0 ) = Rxy( 1, 1 ) = t2( 0 ) / x2;
  Rxy( 0, 1 ) = Rxy( 1, 0 ) = t2( 1 ) / x2;
  Rxy( 1, 0 ) *= -1;
  Rxy( 2, 2 ) = 1;

  Rxy *= R;

  t2 = Rxy * ( P[ 2 ] - P[ 1 ] );
  TCol t3 = Rxy * ( P[ 3 ] - P[ 1 ] );
  TCol t4 = Rxy * ( P[ 0 ] - P[ 1 ] );

  TReal cx = x2 * 0.5;
  TReal cy = ( ( t3( 0 ) * t3( 0 ) ) + ( t3( 1 ) * t3( 1 ) ) - ( x2 * t3( 0 ) ) ) / ( 2.0 * t3( 1 ) );
  TReal Cz = ( ( t3( 1 ) * t4( 2 ) * t4( 2 ) ) + ( t3( 1 ) * ( ( t4( 0 ) * t4( 0 ) ) + ( t4( 1 ) * t4( 1 ) ) - ( x2 * t4( 0 ) ) ) ) - ( t3( 1 ) * t3( 1 ) * t4( 1 ) ) + ( t4( 1 ) * ( ( x2 * t3( 0 ) ) - ( t3( 0 ) * t3( 0 ) ) ) ) ) / ( 2.0 * t3( 1 ) * t4( 2 ) );

  TCol cc, sc;
  cc << cx, cy, TReal( 0 );
  sc << cx, -cy, Cz;

  std::cout << "Circle: " << cc.transpose( ) << std::endl;
  std::cout << "   P1: " << ( cc.transpose( ) * cc ).array( ).sqrt( ).sum( ) << std::endl;
  std::cout << "   P2: " << ( ( cc - t2 ).transpose( ) * ( cc - t2 ) ).array( ).sqrt( ).sum( ) << std::endl;
  std::cout << "   P3: " << ( ( cc - t3 ).transpose( ) * ( cc - t3 ) ).array( ).sqrt( ).sum( ) << std::endl;
  
  std::cout << "Sphere: " << sc.transpose( ) << std::endl;
  std::cout << "   P1: " << ( sc.transpose( ) * sc ).array( ).sqrt( ).sum( ) << std::endl;
  std::cout << "   P2: " << ( ( sc - t2 ).transpose( ) * ( sc - t2 ) ).array( ).sqrt( ).sum( ) << std::endl;
  std::cout << "   P3: " << ( ( sc - t3 ).transpose( ) * ( sc - t3 ) ).array( ).sqrt( ).sum( ) << std::endl;
  std::cout << "   P4: " << ( ( sc - t4 ).transpose( ) * ( sc - t4 ) ).array( ).sqrt( ).sum( ) << std::endl;


  // std::cout << "real: " << real_C.transpose( ) << std::endl;
  // std::cout << ( Rxy.transpose( ) * sc ).transpose( ) << std::endl;
  // std::cout << "comp: " << ( ( Rxy.transpose( ) * sc ) + P[ 1 ] ).transpose( ) << std::endl;



  return( EXIT_SUCCESS );
}

/*
  Rx = numpy.zeros( ( 3, 3 ) )
  Rx[ 0, 0 ] = Rx[ 1, 1 ] = Tx / x2
  Rx[ 0, 1 ] = Rx[ 1, 0 ] = Ty / x2
  Rx[ 1, 0 ] *= -1
  Rx[ 2, 2 ] = 1
  R2 = Rx @ R

  t3 = ( p3 - p1 ) @ R2.T
  t4 = ( p4 - p1 ) @ R2.T
  print( 't3->', t3 )
  print( 't4->', t4 )
  x3 = t3[ 0, 0 ]
  y3 = t3[ 0, 1 ]
  x4 = t4[ 0, 0 ]
  y4 = t4[ 0, 1 ]
  z4 = t4[ 0, 2 ]

  return [ n, R2, numpy.matrix( p1 ), [ x2, x3, y3, x4, y4, z4 ] ]
# end def

real_R, real_C, P = create_points_in_sphere( )

n, Pr, Pt, [ x2, x3, y3, x4, y4, z4 ] = transformation_to_XY(
  P[ 1 ][ 0 ], P[ 2 ][ 0 ], P[ 3 ][ 0 ], P[ 0 ][ 0 ]
  )

# print( '1 -->', ( P[ 1 ][ 0 ] - Pt ) @ Pr.T )
# print( '2 -->', ( P[ 2 ][ 0 ] - Pt ) @ Pr.T, x2 )
# print( '3 -->', ( P[ 3 ][ 0 ] - Pt ) @ Pr.T, x3, y3 )
# print( '4 -->', ( P[ 0 ][ 0 ] - Pt ) @ Pr.T, x4, y4, z4 )
# print( P )

# Circumcircle and circumsphere
cx = x2 * 0.5
cy = ( ( x3 ** 2 ) + ( y3 ** 2 ) - ( x2 * x3 ) ) / ( 2.0 * y3 )
Cz = ( ( y3 * z4 * z4 ) + ( y3 * ( ( x4 ** 2 ) + ( y4 ** 2 ) - ( x2 * x4 ) ) ) - ( y3 * y3 * y4 ) + ( y4 * ( ( x2 * x3 ) - ( x3 ** 2 ) ) ) ) / ( 2.0 * y3 * z4 )
# cc_y = cs_y = ( ( x2 * x3 ) - ( x3 ** 2 ) - ( y3 ** 2 ) ) / ( 2.0 * y3 )
# cs_y *= -1
# cs_z = ( ( y3 * ( ( x4 ** 2 ) + ( y4 ** 2 ) + ( z4 ** 2 ) - ( x2 * x4 ) ) ) - ( y4 * ( ( y3 ** 2 ) + ( x2 * x3 ) - ( x3 ** 2 ) ) ) ) / ( 2.0 * y3 * z4 )

cc = numpy.matrix( [ cx, cy, 0 ] )
sc = numpy.matrix( [ cx, -cy, Cz ] )

print( 'circle --> ', cc, ( cc @ Pr ) + Pt )
print( 'p1 --> ', ( ( ( cc @ Pr ) + Pt - P[ 1 ][ 0 ] ) @ ( ( cc @ Pr ) + Pt - P[ 1 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
print( 'p2 --> ', ( ( ( cc @ Pr ) + Pt - P[ 2 ][ 0 ] ) @ ( ( cc @ Pr ) + Pt - P[ 2 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
print( 'p3 --> ', ( ( ( cc @ Pr ) + Pt - P[ 3 ][ 0 ] ) @ ( ( cc @ Pr ) + Pt - P[ 3 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
print( 'sphere --> ', sc, ( sc @ Pr ) + Pt )
print( 'p1 --> ', ( ( ( sc @ Pr ) + Pt - P[ 1 ][ 0 ] ) @ ( ( sc @ Pr ) + Pt - P[ 1 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
print( 'p2 --> ', ( ( ( sc @ Pr ) + Pt - P[ 2 ][ 0 ] ) @ ( ( sc @ Pr ) + Pt - P[ 2 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
print( 'p3 --> ', ( ( ( sc @ Pr ) + Pt - P[ 3 ][ 0 ] ) @ ( ( sc @ Pr ) + Pt - P[ 3 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )
print( 'p4 --> ', ( ( ( sc @ Pr ) + Pt - P[ 0 ][ 0 ] ) @ ( ( sc @ Pr ) + Pt - P[ 0 ][ 0 ] ).T )[ 0, 0 ] ** 0.5 )

print( '----------------------------------' ) 
print( 'p1 --> ', ( sc @ sc.T )[ 0, 0 ] ** 0.5 )
print( 'p2 --> ', ( ( sc - numpy.matrix( [ x2, 0, 0 ] ) ) @ ( sc - numpy.matrix( [ x2, 0, 0 ] ) ).T )[ 0, 0 ] ** 0.5 )
print( 'p3 --> ', ( ( sc - numpy.matrix( [ x3, y3, 0 ] ) ) @ ( sc - numpy.matrix( [ x3, y3, 0 ] ) ).T )[ 0, 0 ] ** 0.5 )
print( 'p4 --> ', ( ( sc - numpy.matrix( [ x4, y4, z4 ] ) ) @ ( sc - numpy.matrix( [ x4, y4, z4 ] ) ).T )[ 0, 0 ] ** 0.5 )

# cc_C = numpy.matrix( [ cc_x, cc_y, 0 ] )
# cs_C = numpy.matrix( [ cs_x, cs_y, cs_z ] )
# cc_R = ( cc_C @ cc_C.T )[ 0, 0 ] ** 0.5
# cs_R = ( cs_C @ cs_C.T )[ 0, 0 ] ** 0.5

# c_phi = cs_z / cs_R
# s_phi = ( cc_R / cs_R )
# if z4 > 0:
#   s_phi *= -1.0
# # end if
# H = s_phi / cc_R


print( '-------------------------------------------' )
print( 'Real radius:', real_R )
print( 'Real center:', real_C )
print( '-------------------------------------------' )
# print( 'Normal:', n )
# print( 'Circle center:', cc_C )
# print( 'Circle radius:', cc_R )
# print( 'Sphere center:', ( cs_C @ Pr ) + Pt )
# print( 'Sphere radius:', cs_R )
# print( '-------------------------------------------' )
# print( 'Phi:', c_phi, s_phi )
# print( 'H:', H )
# print( '-------------------------------------------' )
*/

// eof - $RCSfile$
