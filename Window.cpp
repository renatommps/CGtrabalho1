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
#include <cmath>        // std::abs
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
    if (_yMin - value >= WINDOW_MIN_VALUE) {
        _yMin -= value;
        _yMax -= value;
    } else {
        double maxValidValue = std::abs(WINDOW_MIN_VALUE) - std::abs(_yMin);
        _yMin -= _yMin;
        _yMax -= _yMin;
    }
}

void Window::moveLeft(double value) {
    if (_xMin - value >= WINDOW_MIN_VALUE) {
        _xMin += value;
        _xMax += value;
    } else {
        double maxValidValue = std::abs(WINDOW_MAX_VALUE) - std::abs(_yMax);
        _xMin += _yMin;
        _xMax += _yMin;
    }
}

void Window::moveRight(double value) {
    if (_xMin + value <= WINDOW_MAX_VALUE) {
        _xMin -= value;
        _xMax -= value;
    } else {
        double maxValidValue = std::abs(WINDOW_MAX_VALUE) - std::abs(_yMax);
        _xMin -= _yMin;
        _xMax -= _yMin;
    }
}

void Window::moveDown(double value) {
    if (_yMax + value <= WINDOW_MAX_VALUE) {
        _yMin += value;
        _yMax += value;
    } else {
        double maxValidValue = std::abs(WINDOW_MAX_VALUE) - std::abs(_yMax);
        _yMin += _yMin;
        _yMax += _yMin;
    }
}

void Window::zoomIn(double value) {
    _xMin += value / 2;
    _xMax -= value / 2;
    _yMin += value / 2;
    _yMax -= value / 2;
}

void Window::zoomOut(double value) {
    _xMin -= value / 2;
    _xMax += value / 2;
    _yMin -= value / 2;
    _yMax += value / 2;
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