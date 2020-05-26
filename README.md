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

 The main function of the `Mandlebrot` class is to perform computation asynchronously. It has a single method `ComputeRow()` that is asynchronous and is called many times in paralell by `ComputeData()`. This resets the value of `_data` which is then passed to the OpenCV library for display. The `Mandlebrot` class also stores the previous bounds in order to be able to zoom out effectively. 

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
 
### Window Class

The main purpose of the `Window` class is to store data about the bounds of an image or complex frame. This is why it takes in a typename variable since it can be used with any numeric precision type such as `ints`, `floats`, `double`, etc. 

```c++
template <typename T>
class Window {

public:
	Window(T x_min, T x_max, T y_min, T y_max)
	: _x_min(x_min), _x_max(x_max), _y_min(y_min), _y_max(y_max)
	{};

// Utility functions for getting the size, width and height of the window
	T Size() const {
		return (Width() * Height());
	}

	T Width() const {
		return (_x_max - _x_min);
	}

	T Height() const {
		return (_y_max - _y_min);
	}

// Getters for the window elements
	T x_min() const {
		return _x_min;
	}

	T x_max() const {
		return _x_max;
	}	

	T y_min() const {
		return _y_min;
	}

	T y_max() const {
		return _y_max;
	}

// Reset all values
	void Reset(T x_min, T x_max, T y_min, T y_max) {
		_x_min = x_min;
		_x_max = x_max;
		_y_min = y_min;
		_y_max = y_max;
	}
  private: 
  	T _x_min, _x_max, _y_min, _y_max;
};


```

### Helper Methods

Also defined in `Mandlebrot.h` are a couple of functional helper methods which do very well-defined single tasks (i.e converting from complex to pixel space, computing the color of a complex number, etc. 

```cpp

std::complex<double> imgPosToComplex(const Window<int> &imgBounds, const Window<double> &complexBounds, int xpos, int ypos);

int numIterUntilEscape(std::complex<double> compNum, int iterMax);

std::tuple<unsigned char, unsigned char, unsigned char> IterToRGB(int iter, int iterMax);

```

### main.cpp

This file contains the entry point for the program. It has two major functions. The `main` function simply creates a `Mandlebrot` object, populates a CV matrix with the object's data and then displays that data as an image. The call back function handles zooming in and out and is called whenever a mouse event occurs. 

**Note**: the `Mandlebrot` object is passed by reference into the callback function so that the callback function can call member functions of the object. After doing so the thread sleeps until a key is pressed (and so no memory is deallocated) so this is a memory safe operation.  


## Rubric Items Addressed


### Loops Functions and I/O

- The program uses many loops (esp in helper functions) in order to compute data about the mandlebrot set

- The program takes in input (in the form of mouse events) and uses the input to do computation

### Object Oriented Programming

- The program is split up into classes that have separate repsonsibiltiies. However, there is not really an 'object' interacting in this class, rather it is a ton of data computation that is facilitated and held responsible by objects

- Access modifiers are specified

- Everything is documented

- The project uses templates to generalize the `Window` class. 

### Memory Management

- The program uses references in function declarations and passes objects by reference

- The program uses shared pointers for the `_data` matrix which is shared between the main loop and the Mandlebrot class. 

### Concurrency

- The program uses futures and `std::async`

- The program uses a timed mutex to prevent concurrent mouse events from interfering with each other. 


