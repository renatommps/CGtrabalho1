/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeometricObject.cpp
 * Author: renato
 * 
 * Created on 14 de Agosto de 2016, 12:06
 */

#include "GeometricObject.h"

GeometricObject::GeometricObject(std::string name) {
    _name = name;
}

GeometricObject::GeometricObject() {
    _name = "Nome padrão";
}

GeometricObject::~GeometricObject() {
}

std::vector<Point> GeometricObject::getPointsVector() {
    return _pointsVector;
}

void GeometricObject::addPointToPointsVector(Point p) {
    _pointsVector.push_back(p);
}

std::string GeometricObject::getName() {
    return _name;
}