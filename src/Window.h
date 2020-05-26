#ifndef WINDOW__H
#define WINDOW__H

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

// Getters and setters for the window elements
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
#endif