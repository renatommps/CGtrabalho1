/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Window.h
 * Author: renato
 *
 * Created on 14 de Agosto de 2016, 22:31
 */

#ifndef WINDOW_H
#define WINDOW_H

/* +++++++++++++++++++++++++++++++ CONSTANTS ++++++++++++++++++++++++++++++++ */
static const float DEFAULT_X_MIN = 0;
static const float DEFAULT_Y_MIN = 0;
static const float DEFAULT_WIDTH = 500;
static const float DEFAULT_HEIGHT = 400;
static const float WINDOW_MIN_VALUE = 0;
static const float WINDOW_MAX_VALUE = 10000;
//static const float X_MAX_DEFAULT = 500;
//static const float Y_MAX_DEFAULT = 400;
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

class Window {
public:
//    Window(float xMin, float yMin, float xMax, float yMax);
    Window(float xMin, float yMin, float width, float height);
    Window();
    virtual ~Window();
    void moveUp(float value);
    void moveLeft(float value);
    void moveRight(float value);
    void moveDown(float value);
    void zoomIn(float Value);
    void zoomOut(float value);
private:
    float _xMin;
    float _yMin;
//    float _xMax;
//    float _yMax;
    float _width;
    float _height;
//    float _xCenter;
//    float _yCenter;
};

#endif /* WINDOW_H */

