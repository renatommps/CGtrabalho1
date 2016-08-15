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


Point::Point(int x, int y) {
    _x = x;
    _y = y;
}

Point::Point() {
    _x = 0;
    _y = 0;
}

Point::~Point() {
}

int Point::getX() {
    return _x;
}

int Point::getY() {
    return _y;
}