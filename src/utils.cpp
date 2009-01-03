/*                                                         
** Copyright (C) 2008 Ricard Marxer <email@ricardmarxer.com>
**                                                                  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or   
** (at your option) any later version.                                 
**                                                                     
** This program is distributed in the hope that it will be useful,     
** but WITHOUT ANY WARRANTY; without even the implied warranty of      
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       
** GNU General Public License for more details.                        
**                                                                     
** You should have received a copy of the GNU General Public License   
** along with this program; if not, write to the Free Software         
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
*/                                                                          

#include "typedefs.h"
#include "debug.h"

#include <Eigen/Core>
#include <Eigen/Array>
#include <Eigen/QR> 

#include <cmath>

#include "utils.h"

void roots(MatrixXR poly, MatrixXC* result) {
  const int N = poly.cols();
  (*result).resize(N-1, 1);
  
  if ( N > 1 ) {
    // Build companion matrix and find its eigenvalues (the root)
    MatrixXR A = MatrixXR::Zero(N - 1, N - 1);
    A.corner( Eigen::BottomLeft, N - 2, N - 2).diagonal().setOnes();
    A.row(0) = -poly.corner( Eigen::TopRight, 1, N - 1 ) / poly(0, 0);
    
    // Get the eigen values
    (*result).set(Eigen::EigenSolver<MatrixXR>(A).eigenvalues());
  }

  reverseCols(result);
}

void reverseCols(MatrixXC* in) { 
  const int N = (*in).cols();
  
  for(int i = 0; i < N / 2; i++ ){
    (*in).col(i).swap((*in).col(N - i - 1));
  }
}

void reverseCols(MatrixXR* in) {
  const int N = (*in).cols();
  
  for(int i = 0; i < N / 2; i++ ){
    (*in).col(i).swap((*in).col(N - i - 1));
  }
}

/*
void tf2zpk(MatrixXC b, MatrixXC a, MatrixXC* zeros, MatrixXC* poles, Real* gain){
  // Return zero, pole, gain (z,p,k) representation from a numerator,
  // denominator representation of a linear filter.
  (*gain) = b(0);
  b /= b[0];
  z = roots(b);
  p = roots(a);
}

def zpk2tf(MatrixXC zeros, MatrixXC poles, Real gain, MatrixXC* b, MatrixXC* a):
    """Return polynomial transfer function representation from zeros
    and poles

    Inputs:

      z, p --- sequences representing the zeros and poles.
      k --- system gain.

    Outputs: (b,a)

      b, a --- numerator and denominator polynomials.
    """
    z = atleast_1d(z)
    k = atleast_1d(k)
    if len(z.shape) > 1:
        temp = poly(z[0])
        b = zeros((z.shape[0], z.shape[1]+1), temp.dtype.char)
        if len(k) == 1:
            k = [k[0]]*z.shape[0]
        for i in range(z.shape[0]):
            b[i] = k[i] * poly(z[i])
    else:
        b = k * poly(z)
    a = poly(p)
    return b, a
*/
