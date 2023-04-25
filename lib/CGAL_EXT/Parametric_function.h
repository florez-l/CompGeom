// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CGAL_EXT__Parametric_function__h__
#define __CGAL_EXT__Parametric_function__h__

#include <cmath>

namespace CGAL_EXT
{
  /**
   */
  template< class _K >
  class Parametric_function
  {
  public:
    using TKernel = _K;
    using Self = Parametric_function;

    using TReal = typename TKernel::FT;
    using TPoint = typename TKernel::Point_3;
    using TVector = typename TKernel::Vector_3;

  public:
    Parametric_function( )
      {
      }
    virtual ~Parametric_function( ) = default;

    virtual TPoint evaluate( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeU( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeU2( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeV( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeV2( const TReal& u, const TReal& v ) const = 0;
    virtual TVector derivativeUV( const TReal& u, const TReal& v ) const = 0;

    virtual TVector normal( const TReal& u, const TReal& v ) const
      {
        TVector n =
          CGAL::cross_product(
            this->derivativeU( u, v ), this->derivativeV( u, v )
            );
        TReal m = std::sqrt( n.squared_length( ) );
        if( m > TReal( 0 ) )
          return( n / m );
        else
          return( n );
      }

    void first_fundamental_form(
      TReal& E, TReal& F, TReal& G,
      const TReal& u, const TReal& v
      ) const
      {
        TVector ru = this->derivativeU( u, v );
        TVector rv = this->derivativeV( u, v );

        E = ru.squared_length( );
        G = rv.squared_length( );
        F = ru * rv;
      }

    void second_fundamental_form(
      TReal& L, TReal& M, TReal& N,
      const TReal& u, const TReal& v
      ) const
      {
        TVector ru2 = this->derivativeU2( u, v );
        TVector rv2 = this->derivativeV2( u, v );
        TVector ruv = this->derivativeUV( u, v );
        TVector n = this->normal( u, v );

        L = ru2 * n;
        M = ruv * n;
        N = rv2 * n;
      }

    virtual std::pair< TReal, TReal > curvatures(
      const TReal& u, const TReal& v
      ) const
      {
        TReal E, F, G, L, M, N;
        this->first_fundamental_form( E, F, G, u, v );
        this->second_fundamental_form( L, M, N, u, v );

        std::cout
          << E << " " << F << " " << G << " : "
          << L << " " << M << " " << N << std::endl;


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
