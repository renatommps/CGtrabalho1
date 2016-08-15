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

//Window::Window(float xMin, float yMin, float xMax, float yMax) {
//    if (xMax > xMin) {
//        _xMin = xMin;
//        _xMax = xMax;
//    } else {
//        _xMin = X_MIN_DEFAULT;
//        _xMax = X_MAX_DEFAULT;
//    }
//    if (yMax > yMin) {
//        _yMin = yMin;
//        _yMax = yMax;
//    } else {
//        _yMin = Y_MIN_DEFAULT;
//        _yMax = Y_MAX_DEFAULT;
//    }
//    _width = _xMax - _xMin;
//    _height = _yMax - _yMin;
//    _xCenter = _xMin - (_width / 2);
//    _yCenter = _yMin - (_height / 2);
//}

Window::Window(float xMin, float yMin, float width, float height){
    xMin > WINDOW_MIN_VALUE ? _xMin = xMin : _xMin = DEFAULT_X_MIN;
    yMin > WINDOW_MIN_VALUE ? _yMin = yMin : yMin = DEFAULT_Y_MIN;
}

Window::Window() {
    _xMin = DEFAULT_X_MIN;
    _yMin = DEFAULT_Y_MIN;
    _width = DEFAULT_WIDTH;
    _height = DEFAULT_HEIGHT;
//    _xMax = X_MAX_DEFAULT;
//    _yMax = Y_MAX_DEFAULT;
//    _width = _xMax - _xMin;
//    _height = _yMax - _yMin;
//    _xCenter = _xMin - (_width / 2);
//    _yCenter = _yMin - (_height / 2);
}

Window::~Window() {
}

void Window::moveUp(float value) {

}

void Window::moveLeft(float value) {

}

void Window::moveRight(float value) {

}

void Window::moveDown(float value) {

}

void Window::zoomIn(float value) {
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

void Window::zoomOut(float value) {
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