// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CGAL_EXT__Sphere_function__h__
#define __CGAL_EXT__Sphere_function__h__

#include <CGAL_EXT/Parametric_function.h>

namespace CGAL_EXT
{
  /**
   */
  template< class _K >
  class Sphere_function
    : public CGAL_EXT::Parametric_function< _K >
  {
  public:
    using TKernel = _K;
    using Self = Sphere_function;
    using Superclass = CGAL_EXT::Parametric_function< TKernel >;

    using TReal = typename TKernel::FT;
    using TPoint = typename TKernel::Point_3;
    using TVector = typename TKernel::Vector_3;

  public:
    Sphere_function( )
      : Superclass( )
      {
      }
    virtual ~Sphere_function( ) = default;

    double radius( ) const
      {
        return( double( this->m_Radius ) );
      }
    void set_radius( const double& r )
      {
        this->m_Radius = TReal( r );
      }

    virtual TPoint evaluate( const TReal& u, const TReal& v ) const override
      {
        double cu = std::cos( double( u ) );
        double su = std::sin( double( u ) );
        double cv = std::cos( double( v ) );
        double sv = std::sin( double( v ) );
        double r = double( this->m_Radius );

        return( TPoint( r * cu * sv, r * su * sv, r * cv ) );
      }
    virtual TVector derivativeU( const TReal& u, const TReal& v ) const override
      {
        double cu = std::cos( double( u ) );
        double su = std::sin( double( u ) );
        double cv = std::cos( double( v ) );
        double sv = std::sin( double( v ) );
        double r = double( this->m_Radius );

        return( TVector( -r * su * sv, r * cu * sv, double( 0 ) ) );
      }
    virtual TVector derivativeU2( const TReal& u, const TReal& v ) const override
      {
        double cu = std::cos( double( u ) );
        double su = std::sin( double( u ) );
        double cv = std::cos( double( v ) );
        double sv = std::sin( double( v ) );
        double r = double( this->m_Radius );
        
        return( TVector( -r * cu * sv, -r * su * sv, double( 0 ) ) );
      }
    virtual TVector derivativeV( const TReal& u, const TReal& v ) const override
      {
        double cu = std::cos( double( u ) );
        double su = std::sin( double( u ) );
        double cv = std::cos( double( v ) );
        double sv = std::sin( double( v ) );
        double r = double( this->m_Radius );

        return( TVector( r * cu * cv, r * su * cv, -r * sv ) );
      }
    virtual TVector derivativeV2( const TReal& u, const TReal& v ) const override
      {
        double cu = std::cos( double( u ) );
        double su = std::sin( double( u ) );
        double cv = std::cos( double( v ) );
        double sv = std::sin( double( v ) );
        double r = double( this->m_Radius );

        return( TVector( -r * cu * sv, -r * su * sv, -r * cv ) );
      }
    virtual TVector derivativeUV( const TReal& u, const TReal& v ) const override
      {
        double cu = std::cos( double( u ) );
        double su = std::sin( double( u ) );
        double cv = std::cos( double( v ) );
        double sv = std::sin( double( v ) );
        double r = double( this->m_Radius );

        return( TVector( -r * su * cv, r * cu * cv, double( 0 ) ) );
      }

    virtual TVector normal( const TReal& u, const TReal& v ) const override
      {
        double cu = std::cos( double( u ) );
        double su = std::sin( double( u ) );
        double cv = std::cos( double( v ) );
        double sv = std::sin( double( v ) );

        return( TVector( -cu * sv, -su * sv, -cv ) );
      }

    virtual std::pair< TReal, TReal > curvatures(
      const TReal& u, const TReal& v
      ) const override
      {
        TReal K = TReal( 1 ) / this->m_Radius;
        TReal H = ( K + TReal( 1 ) ) * TReal( 0.5 );
        return( std::make_pair( K, H ) );
      }

  protected:
    TReal m_Radius { 1 };
  };
} // end namespace

// TODO: #include <CGAL_EXT/Sphere_function.hxx>

#endif // __CGAL_EXT__Sphere_function__h__

// eof - $RCSfile$
