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

#include "peakcog.h"

#include "utils.h"

using namespace std;

// import most common Eigen types 
using namespace Eigen;

PeakCOG::PeakCOG(int fftLength, int bandwidth) :
  _fftLength(fftLength),
  _bandwidth(bandwidth)
{
  
  DEBUG("PeakCOG: Constructor fftLength: " << _fftLength);
  
  setup();
}

PeakCOG::~PeakCOG() {}


void PeakCOG::setup() {
  // Prepare the buffers
  DEBUG("PeakCOG: Setting up...");
 
  reset();

  DEBUG("PeakCOG: Finished set up...");
}


void PeakCOG::process(const MatrixXC& fft, const MatrixXR& peakPos, MatrixXR* peakCog) {
  DEBUG("PeakCOG: Processing windowed");
  const int rows = fft.rows();
  const int cols = fft.cols();
  const int halfCols = min((int)ceil(_fftLength / 2.0), cols);
  const int peakCount = peakPos.cols();

  DEBUG("PeakCOG: Spectrum resized rows: " << rows << " halfCols: " << halfCols);
  
  _spectrumAbs2 = fft.block(0, 0, rows, halfCols).cwise().abs2();
  unwrap(fft.block(0, 0, rows, halfCols).cwise().angle(), &_spectrumArg);
  derivate(_spectrumArg, &_spectrumArgDeriv);
  
  (*peakCog).resize(rows, peakCount);
  (*peakCog).setZero();

  for(int row = 0; row < rows; row++) {
    for(int i = 0; i < peakCount; i++){     
      if (peakPos(row, i) != -1) {
        int start = max(0, (int)floor(peakPos(row, i) - _bandwidth / 2));
        int end = min(halfCols, (int)ceil(peakPos(row, i) + _bandwidth / 2));

        (*peakCog)(row, i) = ((-_spectrumArgDeriv).block(row, start, 1, end).cwise() * _spectrumAbs2.block(row, start, 1, end)).sum() / _spectrumAbs2.block(row, start, 1, end).sum();

      }
      
    }
  }
  
  DEBUG("PeakCOG: Finished Processing");
}

void PeakCOG::reset() {
  // Initial values
}
