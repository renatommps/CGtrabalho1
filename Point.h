/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Point.h
 * Author: renato
 *
 * Created on 14 de Agosto de 2016, 12:19
 */

#ifndef POINT_H
#define POINT_H

class Point {
public:
    Point(float x, float y);
    Point();
    virtual ~Point();
    float getX();
    float getY();
private:
    float _x;
    float _y;
};

#endif /* POINT_H */

