/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Window.cpp
 * Author: renato
 * 
 * Created on 14 de Agosto de 2016, 22:31
 */

#include <algorithm>    // std::min, std::max
#include "Window.h"

Window::Window(double xMin, double yMin, double xMax, double yMax) {
    if (xMax > xMin) {
        _xMin = xMin;
        _xMax = xMax;
    } else {
        _xMin = X_MIN_DEFAULT;
        _xMax = X_MAX_DEFAULT;
    }

    if (yMax > yMin) {
        _yMin = yMin;
        _yMax = yMax;
    } else {
        _yMin = Y_MIN_DEFAULT;
        _yMax = Y_MAX_DEFAULT;
    }
}

Window::Window() {
    _xMin = X_MIN_DEFAULT;
    _yMin = Y_MIN_DEFAULT;
    _xMax = X_MAX_DEFAULT;
    _yMax = Y_MAX_DEFAULT;
}

Window::~Window() {
}

void Window::moveUp(double value) {

}

void Window::moveLeft(double value) {

}

void Window::moveRight(double value) {

}

void Window::moveDown(double value) {

}

void Window::zoomIn(double value) {
    //    float xMaxZoomOut = _width - _xMin - value;
    //    float yMaxZoomOut = _height - _yMin - value;
    //    
    //    if (xMaxZoomOut > WINDOW_MIN_VALUE) {
    //        _width -= value;
    //    } else {
    //        _width -= maxZoomIn;
    //    }
    //    
    //    
    //    
    //    float maxZoomIn = std::min(xMaxZoomIn, yMaxZoomIn);
    //
    //    if (maxZoomIn <= WINDOW_MIN_VALUE) {
    //        return;
    //    }
    //
    //    if ((xMaxZoomIn - value > WINDOW_MIN_VALUE) && (yMaxZoomIn - value > WINDOW_MIN_VALUE)) {
    //        _width -= value;
    //        _height -= value;
    //    } else {
    //        _width -= maxZoomIn;
    //        _height -= maxZoomIn;
    //    }
}

void Window::zoomOut(double value) {
    //    float xMaxZoomIn = _xMin + _width + value;
    //    float yMaxZoomIn = _yMin + _height + value;
    //    float maxZoomIn = std::min(xMaxZoomIn, yMaxZoomIn);
    //
    //    if (maxZoomIn > WINDOW_MIN_VALUE) {
    //        return;
    //    }
    //
    //    if ((xMaxZoomIn - value > WINDOW_MIN_VALUE) && (yMaxZoomIn - value > WINDOW_MIN_VALUE)) {
    //        _width -= value;
    //        _height -= value;
    //    } else {
    //        _width -= maxZoomIn;
    //        _height -= maxZoomIn;
    //    }
}

double Window::getXmin() {
    return _xMin;
}

double Window::getYmin() {
    return _yMin;
}

double Window::getXmax() {
    return _xMax;
}

double Window::getYmax() {
    return _yMax;
}