#include "catch.hpp"
#include <numcpp.h>
using namespace numcpp;

TEST_CASE( "numcpp/fft/dct", "DCT test" )
{

  {
    size_t N = 8;
    Vector<double> x = range(0,N);

    auto y = dct(x);
    auto z = idct(y);

    REQUIRE( norm(x - z) < 1e-6 );
  }

  {
    size_t N = 8;
    Vector<float> x = range(0,N);

    auto y = dct(x);
    auto z = idct(y);

    REQUIRE( norm(x - z) < 1e-6 );
  }

}

TEST_CASE( "numcpp/fft/fft", "FFT test" )
{

  {
    size_t N = 8;
    Vector<double> x = range(0,N);

    auto y = fft(x);
    auto z = ifft(y);

    REQUIRE( norm(x - z) < 1e-6 );
  }

  {
    size_t N = 8;
    Vector<float> x = range(0,N);

    auto y = fft(x);
    auto z = ifft(y);

    REQUIRE( norm(x - z) < 1e-6 );
  }

}

TEST_CASE( "numcpp/fft/nfft", "NFFT test" )
{
  size_t m = 4;
  double sigma = 2.0;

  { // 1D
    size_t N = 16;
    size_t M = N;
    
	Vector<double> x = linspace(-0.5, 0.5, M);
	Vector<cdouble> fHat = range(0, M);

    auto f = ndftAdjoint(fHat, x, N);
    auto fApprox = nfftAdjoint(fHat, x, N, m, sigma);

    REQUIRE( nrmsd(f, fApprox) < 1e-6 );

    auto g = ndft(f, x);
    auto gApprox = nfft(f, x, m, sigma);

    REQUIRE( nrmsd(g, gApprox) < 1e-6 );
  }

  { // 2D
    size_t N = 8;
    size_t M = N*N;

    Matrix<double> x = reshape(linspace(-0.5,0.5,2*M), M, 2);
	Vector<cdouble> fHat = range(0, M);

    auto f = ndftAdjoint<2>(fHat, x, {N,N});
    auto fApprox = nfftAdjoint(fHat, x, {N,N}, m, sigma);

    REQUIRE( nrmsd(f, fApprox) < 1e-6 );

    auto g = ndft(f, x);
    auto gApprox = nfft(f, x, m, sigma);

    REQUIRE( nrmsd(g, gApprox) < 1e-6 );
  }

  { // 3D
    size_t N = 8;
    size_t M = N*N*N;

    Matrix<double> x = reshape(linspace(-0.5,0.5,3*M), M, 3);
    Vector<cdouble> fHat = range(0, M);

    auto f = ndftAdjoint<3>(fHat, x, {N,N,N});
    auto fApprox = nfftAdjoint(fHat, x, {N,N,N}, m, sigma);

    REQUIRE( nrmsd(f, fApprox) < 1e-6 );

    auto g = ndft(f, x);
    auto gApprox = nfft(f, x, m, sigma);

    REQUIRE( nrmsd(g, gApprox) < 1e-6 );
  }



}

