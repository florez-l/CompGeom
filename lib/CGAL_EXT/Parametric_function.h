// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CGAL_EXT__Parametric_function__h__
#define __CGAL_EXT__Parametric_function__h__

#include <map>
#include <CGAL/HalfedgeDS_items_decorator.h>

namespace CGAL_EXT
{
  /**
   */
  template< class _K >
  class Parametric_function
  {
  public:
    using Self = Parametric_function;

  public:
    Parametric_function( );
    virtual ~Parametric_function( ) = default;

    virtual TPoint evaluate( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeU( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeU2( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeV( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeV2( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeUV( const TReal& u, const TReal& v ) const = 0;
    virtual TVector normal( const TReal& u, const TReal& v ) const = 0;
    

    void first_fundamental_form(
      TReal& E, TReal& F, TReal& G,
      const TReal& u, const TReal& v
      ) const
      {
      }

    void second_fundamental_form(
      TReal& L, TReal& M, TReal& N,
      const TReal& u, const TReal& v
      ) const
      {
      }

    std::pair< TReal, TReal > curvatures(
      const TReal& u, const TReal& v
      ) const
      {
        TReal E, F, G, L, M, N;
        this->first_fundamental_form( E, F, G, u, v );
        this->second_fundamental_form( L, M, N, u, v );

        TReal K = ( ( L * N ) - ( M * M ) ) / ( ( E * G ) - ( F * F ) );
        TReal H =
          ( ( L * G ) - ( 2.0 * M * F ) + ( N * E ) )
          /
          ( 2.0 * ( ( E * G ) - ( F * F ) ) );

        return( std::make_pair( K, H ) );
      }

  protected:
  };
} // end namespace

// TODO: #include <CGAL_EXT/Parametric_function.hxx>

#endif // __CGAL_EXT__Parametric_function__h__

// eof - $RCSfile$
