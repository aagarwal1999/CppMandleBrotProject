# CPPND: Capstone Mandlebrot Fractal Project With Zoom! 

This project was made as a part of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

The project itself is a Mandlebrot fractal generator with zoom capabilities. It is also a multithreaded program that asynchronously computes the Mandlebrot fractal in paralell. To use the program simply run the executable. To zoom, click as to where you want to zoom. To zoom out, simply right click or double click. 

C++ concepts used in this project:
- Shared Pointers
- Timed Mutexes
- Futures
- Object Oriented Programming
- Bernstein Polynomials

For the majority of the Mandlebrot Fractal code, [this article](https://solarianprogrammer.com/2013/02/28/mandelbrot-set-cpp-11/) was used as a reference: 


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* opencv >= 4.1
  * Linux: https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
  * Mac: https://docs.opencv.org/master/d0/db2/tutorial_macos_install.html
  * Windows: https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html

## Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Mandlebrot`.


## File Structure

The code consists of three main parts: 

```
Window.h and Window.cpp

Mandlebrot.h and Mandlebrot.cpp

main.cpp

```
Only the file `main.cpp` interacts with the openCV library. The rest of the computation is done through `Mandlebrot.cpp` which contains the entire implementation of the project as well as a couple of helper functions. The class `Window` is used to control bounds for the project. 

### Mandlebrot Class: 

 ```c++
 
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
 
 
 
 
 ```



