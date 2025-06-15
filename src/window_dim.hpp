#pragma once

template <typename T>
class WindowDim {
  T _x_min, _x_max, _y_min, _y_max;
  double _zoom_level;

 public:
  WindowDim(T x_min, T x_max, T y_min, T y_max)
      : _x_min(x_min), _x_max(x_max), _y_min(y_min), _y_max(y_max), _zoom_level(1.0) {}

  // Utility functions for getting the size, width and height of the window
  T size() const { return (width() * height()); }

  T width() const { return (_x_max - _x_min); }

  T height() const { return (_y_max - _y_min); }

  T get_zoom_level() const { return (_zoom_level); }

  // Getters and setters for the window elements
  T x_min() const { return _x_min; }

  void x_min(T x_min) { _x_min = x_min; }

  T x_max() const { return _x_max; }

  void x_max(T x_max) { _x_max = x_max; }

  T y_min() const { return _y_min; }

  void y_min(T y_min) { _y_min = y_min; }

  T y_max() const { return _y_max; }

  void y_max(T y_max) { _y_max = y_max; }

  // Reset all values
  void reset(T x_min, T x_max, T y_min, T y_max) {
    _x_min = x_min;
    _x_max = x_max;
    _y_min = y_min;
    _y_max = y_max;
  }

  void reset(T x_min, T x_max, T y_min, T y_max, double zoom_level) {
    _x_min      = x_min;
    _x_max      = x_max;
    _y_min      = y_min;
    _y_max      = y_max;
    _zoom_level = zoom_level;
  }
};
