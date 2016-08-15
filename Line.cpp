/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Line.cpp
 * Author: renato
 * 
 * Created on 14 de Agosto de 2016, 12:50
 */

#include "Line.h"

Line::Line(std::string name, Point p1, Point p2) : GeometricObject(name) {
    _p1 = p1;
    _p2 = p2;
    addPointToPointsList(_p1);
    addPointToPointsList(_p2);
}

Line::Line(std::string name, int xp1, int yp1, int xp2, int yp2) : GeometricObject(name) {
    _p1 = Point(xp1, yp1);
    _p2 = Point(xp2, yp2);
    addPointToPointsList(_p1);
    addPointToPointsList(_p2);
}

Line::Line() : GeometricObject() {}

Line::~Line() {
}