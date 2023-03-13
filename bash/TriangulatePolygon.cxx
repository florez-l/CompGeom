// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <string>

// -------------------------------------------------------------------------
template< class _PIt, class _TIt >
void ReadOBJ( const std::string& fname, _PIt points, _TIt polygon );

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  if( argc < 3 )
  {
    std::cerr
      << "Usage: " << argv[ 0 ] << " input.obj output.obj"
      << std::endl;
    return( EXIT_FAILURE );
  } // end if

  return( EXIT_SUCCESS );
}

// -------------------------------------------------------------------------
template< class _PIt, class _TIt >
void ReadOBJ( const std::string& fname, _PIt points, _TIt polygon )
{
}

// eof - $RCSfile$
