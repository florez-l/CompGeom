// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>

#include <CGAL/Cartesian.h>

#include <CGAL_EXT/Sphere_function.h>

// -------------------------------------------------------------------------
// Types
using TKernel = CGAL::Cartesian< double >;
using TFunction = CGAL_EXT::Sphere_function< TKernel >;

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  TFunction f;
  f.set_radius( 3 );

  std::cout << f.evaluate( 0, 0 ) << std::endl;
  std::cout << f.normal( 0, 0 ) << std::endl;
  std::cout << f.curvatures( 0, 0 ).first << std::endl;
  std::cout << f.curvatures( 0, 0 ).second << std::endl;

  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
