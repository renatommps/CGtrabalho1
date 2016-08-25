/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Point.cpp
 * Author: renato
 * 
 * Created on 14 de Agosto de 2016, 12:19
 */

#include "Point.h"

Point::Point(double x, double y, double z) {
    _x = x;
    _y = y;
    _z = z;
}

Point::Point(double x, double y) {
    _x = x;
    _y = y;
    _z = 1;
}

Point::Point() {
    _x = 0;
    _y = 0;
    _z = 1;
}

Point::~Point() {
}

double Point::getX() {
    return _x;
}

double Point::getY() {
    return _y;
}

double Point::getZ(){
    return _z;
}