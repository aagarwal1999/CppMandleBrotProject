#ifndef MANDLE_H
#define MANDLE_H

#include <string>
#include <deque>
#include <thread>
#include <memory>
#include <complex>
#include <tuple>
#include "opencv2/highgui/highgui.hpp"
#include "Window.h"

class Mandlebrot
{
public:
    // constructor / desctructor
	Mandlebrot(Window<int> &imgBounds, Window<double> &complexBounds);
    
    // getters and setters
    // shared pointer amongst class and main.cpp 
    std::shared_ptr<void> getImgData(){ return _data; };
    int ImgHeight();
    int ImgWidth();

    // typical behaviour method
    void ComputeData(Window<int> bounds);
    void ZoomIn(int xpos, int ypos);
    void ZoomOut();
    void ComputeRow(int row);
    void ComputeData();
    void ComputeNewBounds(int xpos, int ypos);
    
    // mutex to control the data variable
    std::timed_mutex mtxData;

private:
    // typical behaviour methods
    // stack of previous complex frame bounds for zoom out
    std::deque<Window<double>> _prevBounds;
    // bounds of the image
    Window<int> _imgBounds;
    // bounds of the current complex frame
    Window<double> _currBounds;
    
    // a pointer to the data used by this class
    std::shared_ptr<unsigned char[]> _data;
    
    
};


// Helper Functions 

std::complex<double> imgPosToComplex(const Window<int> &imgBounds, const Window<double> &complexBounds, int xpos, int ypos);

int numIterUntilEscape(std::complex<double> compNum, int iterMax);

std::tuple<unsigned char, unsigned char, unsigned char> IterToRGB(int iter, int iterMax);


#endif