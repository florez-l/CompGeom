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

real_R, real_C, points = create_points_in_sphere( )
n, Pr, Pt, [ x2, x3, y3, x4, y4, z4 ] = transformation_to_XY(
  numpy.matrix( points[ 1 ] ),
  numpy.matrix( points[ 2 ] ),
  numpy.matrix( points[ 3 ] ),
  numpy.matrix( points[ 0 ] )
  )
Ht = numpy.zeros( ( 4, 4 ) )

Ht[ 0, 0 ] = Pr[ 0, 0 ]
Ht[ 0, 1 ] = Pr[ 0, 1 ]
Ht[ 0, 2 ] = Pr[ 0, 2 ]

Ht[ 1, 0 ] = Pr[ 1, 0 ]
Ht[ 1, 1 ] = Pr[ 1, 1 ]
Ht[ 1, 2 ] = Pr[ 1, 2 ]

Ht[ 2, 0 ] = Pr[ 2, 0 ]
Ht[ 2, 1 ] = Pr[ 2, 1 ]
Ht[ 2, 2 ] = Pr[ 2, 2 ]

Qt = Pr @ ( -Pt.T )

Ht[ 0, 3 ] = Qt[ 0 ]
Ht[ 1, 3 ] = Qt[ 1 ]
Ht[ 2, 3 ] = Qt[ 2 ]

Ht[ 3, 3 ] = 1

view_axes = vtk.vtkAxesActor( )
view_axes.SetTotalLength( 200, 200, 200 )
view_axes.SetXAxisLabelText( '' )
view_axes.SetYAxisLabelText( '' )
view_axes.SetZAxisLabelText( '' )

view_sphere = vtk.vtkSphereSource( )
view_sphere.SetCenter( real_C )
view_sphere.SetRadius( real_R )
view_sphere.SetPhiResolution( 50 )
view_sphere.SetThetaResolution( 50 )

view_sphere_mapper = vtk.vtkPolyDataMapper( )
view_sphere_mapper.SetInputConnection( view_sphere.GetOutputPort( ) )

view_sphere_actor = vtk.vtkActor( )
view_sphere_actor.SetMapper( view_sphere_mapper )
view_sphere_actor.GetProperty( ).SetColor( 1, 0.5, 0.75 )
view_sphere_actor.GetProperty( ).SetOpacity( 0.85 )

view_points = vtk.vtkPoints( )
view_points.InsertNextPoint( points[ 0 ] )
view_points.InsertNextPoint( points[ 1 ] )
view_points.InsertNextPoint( points[ 2 ] )
view_points.InsertNextPoint( points[ 3 ] )

view_cells = vtk.vtkCellArray( )
view_cells.InsertNextCell( 3 )
view_cells.InsertCellPoint( 0 )
view_cells.InsertCellPoint( 1 )
view_cells.InsertCellPoint( 2 )
view_cells.InsertNextCell( 3 )
view_cells.InsertCellPoint( 1 )
view_cells.InsertCellPoint( 2 )
view_cells.InsertCellPoint( 3 )
view_cells.InsertNextCell( 3 )
view_cells.InsertCellPoint( 0 )
view_cells.InsertCellPoint( 3 )
view_cells.InsertCellPoint( 2 )
view_cells.InsertNextCell( 3 )
view_cells.InsertCellPoint( 1 )
view_cells.InsertCellPoint( 3 )
view_cells.InsertCellPoint( 0 )

view_mesh = vtk.vtkPolyData( )
view_mesh.SetPoints( view_points )
view_mesh.SetPolys( view_cells )

view_mapper = vtk.vtkPolyDataMapper( )
view_mapper.SetInputData( view_mesh )

view_actor = vtk.vtkActor( )
view_actor.SetMapper( view_mapper )

view_transform = vtk.vtkTransform( )
view_transform.SetMatrix( list( itertools.chain( *Ht.tolist( ) ) ) )

view_transform_filter = vtk.vtkTransformFilter( )
view_transform_filter.SetTransform( view_transform )
view_transform_filter.SetInputData( view_mesh )

view_transform_mapper = vtk.vtkPolyDataMapper( )
view_transform_mapper.SetInputConnection( view_transform_filter.GetOutputPort( ) )

view_transform_actor = vtk.vtkActor( )
view_transform_actor.SetMapper( view_transform_mapper )

ren = vtk.vtkRenderer( )
ren.AddActor( view_axes )
ren.AddActor( view_actor )
ren.AddActor( view_sphere_actor )
ren.AddActor( view_transform_actor )

win = vtk.vtkRenderWindow( )
win.AddRenderer( ren )
win.SetSize( 800, 800 )

rwi = vtk.vtkRenderWindowInteractor( )
rwi.SetRenderWindow( win )
rwi.Initialize( )
rwi.Start( )


  
# real_R, real_C, P = create_points_in_sphere( )

# n, Pr, Pt, [ x2, x3, y3, x4, y4, z4 ] = transformation_to_XY(
#   P[ 1 ][ 0 ], P[ 2 ][ 0 ], P[ 3 ][ 0 ], P[ 0 ][ 0 ]
#   )

# # print( '1 -->', ( P[ 1 ][ 0 ] - Pt ) @ Pr.T )
# # print( '2 -->', ( P[ 2 ][ 0 ] - Pt ) @ Pr.T, x2 )
# # print( '3 -->', ( P[ 3 ][ 0 ] - Pt ) @ Pr.T, x3, y3 )
# # print( '4 -->', ( P[ 0 ][ 0 ] - Pt ) @ Pr.T, x4, y4, z4 )
# # print( P )

# # Circumcircle and circumsphere
# cx = x2 * 0.5
# cy = ( ( x3 ** 2 ) + ( y3 ** 2 ) - ( x2 * x3 ) ) / ( 2.0 * y3 )
# Cz = ( ( y3 * z4 * z4 ) + ( y3 * ( ( x4 ** 2 ) + ( y4 ** 2 ) - ( x2 * x4 ) ) ) - ( y3 * y3 * y4 ) + ( y4 * ( ( x2 * x3 ) - ( x3 ** 2 ) ) ) ) / ( 2.0 * y3 * z4 )
# # cc_y = cs_y = ( ( x2 * x3 ) - ( x3 ** 2 ) - ( y3 ** 2 ) ) / ( 2.0 * y3 )
# # cs_y *= -1
# # cs_z = ( ( y3 * ( ( x4 ** 2 ) + ( y4 ** 2 ) + ( z4 ** 2 ) - ( x2 * x4 ) ) ) - ( y4 * ( ( y3 ** 2 ) + ( x2 * x3 ) - ( x3 ** 2 ) ) ) ) / ( 2.0 * y3 * z4 )

# cc = numpy.matrix( [ cx, cy, 0 ] )
# sc = numpy.matrix( [ cx, Cz, -cy ] )

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
