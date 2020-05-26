#include <iostream>
#include <vector>
#include <chrono>
#include <future>
#include "Mandlebrot.h"

// constants for the class
int MAX_ZOOM = 10;
int ITER_MAX = 50;

Mandlebrot::Mandlebrot(Window<int> &imgBounds, Window<double> &complexBounds) : _imgBounds(imgBounds), _currBounds(complexBounds) {
  
   std::shared_ptr<unsigned char[]> sp(new unsigned char[3 * _imgBounds.Size()]); // 3 channels r/g/b times number of pixels
   _data = sp;
   this->ComputeData();
  
}

int Mandlebrot::ImgHeight(){
 
  return _imgBounds.Height();
}

int Mandlebrot::ImgWidth(){
 
  return _imgBounds.Width();
}


void Mandlebrot::ZoomIn(int xpos, int ypos)
{
  // max number of zoom frames
  if(_prevBounds.size() >= MAX_ZOOM){
    return;
  }
  ComputeNewBounds(xpos, ypos);
  ComputeData();
     
}

void Mandlebrot::ZoomOut()
{
  // if no way to zoom out
  if(_prevBounds.empty()){
    return;
  }
  
  _currBounds = _prevBounds.back();
  _prevBounds.pop_back();
  ComputeData();
  
}

void Mandlebrot::ComputeNewBounds(int xpos, int ypos){
  
  // moves from pixel space to complex space
  std::complex<double> compPos = imgPosToComplex(_imgBounds, _currBounds, xpos, ypos);
  
  // computes a 4x zoom
  double newWidth = _currBounds.Width() / 4.0;
  double newHeight = _currBounds.Height() / 4.0;
  double midX = compPos.real();
  double midY = compPos.imag();
  
  // edge cases and boundary conditions
  if( (midX - newWidth / 2.0) < _currBounds.x_min()){
    
    midX = _currBounds.x_min() + newWidth / 2.0;
  }
  else if((midX + newWidth/2.0) > _currBounds.x_max()){
     midX = _currBounds.x_max() - newWidth / 2.0;
  }
  
  if( (midY - newHeight / 2.0) < _currBounds.y_min()){
    
    midY = _currBounds.y_min() + newHeight / 2.0;
  }
  else if((midY - newHeight/2.0) > _currBounds.y_max()){
     midY = _currBounds.y_max() - newHeight / 2.0;
  }
  
  // puts old bounds in the prev bounds array (in case of zoom out)
  _prevBounds.push_back(_currBounds);
  
  // creates a new bounds object with the new bounds
  _currBounds = Window<double>(midX - newWidth / 2.0, midX + newWidth / 2.0, midY - newHeight / 2.0, midY + newHeight / 2.0);

}

void Mandlebrot::ComputeData() {
  
  int xmin = _imgBounds.x_min();
  int xmax = _imgBounds.x_max();
  std::vector<std::future<void>> futures;
  for(int i = xmin; i < xmax; i++){
    // asycnhronously computes each row
    std::future<void> ftr = std::async(&Mandlebrot::ComputeRow, this, i);
    futures.push_back(std::move(ftr));
    
  }
  
  for (auto & ftr : futures){
    ftr.wait();
  }
  return;
  
}

void Mandlebrot::ComputeRow(int row) {
  int ymin = _imgBounds.y_min();
  int ymax = _imgBounds.y_max();
  int rowStart = row * _imgBounds.Width() * 3;
  for(int i = ymin; i < ymax; i++){
    
    // moves from pixel to complex space
    std::complex<double> comp = imgPosToComplex(_imgBounds, _currBounds, i, row);
    
    int iter = numIterUntilEscape(comp, ITER_MAX);
    
    std::tuple<unsigned char, unsigned char, unsigned char> rgb = IterToRGB(iter, ITER_MAX);
    
    // sets the data values
    // NOTE: no need for mutexes since every thread will be modifying its own row exclusively. 
    // since images are bgr not rgb
    _data[rowStart + 3 * i] = std::get<2>(rgb);
    _data[rowStart + 3 * i + 1] = std::get<1>(rgb);
    _data[rowStart + 3 * i + 2] = std::get<0>(rgb);
    
  }
  
}

std::complex<double> imgPosToComplex(const Window<int> &imgBounds, const Window<double> &complexBounds, int xpos, int ypos){
  
  double xval = (double)xpos / (double)imgBounds.Width() * complexBounds.Width() + complexBounds.x_min();
  
  double yval = (1 - (double)ypos / (double)imgBounds.Height()) * complexBounds.Height() + complexBounds.y_min();
  
  std::complex<double> aux(xval, yval);

  return aux;
}
  
  
  
int numIterUntilEscape(std::complex<double> compNum, int iterMax){
 
  std::complex<double> z(0);
  int iter = 0;
  while(std::abs(z) < 2 && iter < iterMax){

    z = z * z + compNum;
    ++iter; 
  }

  return iter;
   
}

// uses berstein polynomials to compute the rgb values. 
// unsigned chars are 8-bit integers. 
std::tuple<unsigned char, unsigned char, unsigned char> IterToRGB(int iter, int iterMax){
  
  double t = (double)iter/(double)iterMax;
  
  unsigned char r = (unsigned char)(9*(1-t)*t*t*t*255);
  unsigned char g = (unsigned char)(15*(1-t)*(1-t)*t*t*255);
  unsigned char b =  (unsigned char)(8.5*(1-t)*(1-t)*(1-t)*t*255);
  
  return std::tuple<unsigned char, unsigned char, unsigned char>(r, g, b);
  
}
  