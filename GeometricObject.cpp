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

GeometricObject::GeometricObject() {}

GeometricObject::~GeometricObject() {
}

std::list<Point>  GeometricObject::getPointsList() {
    return _pointsList;
}

void GeometricObject::addPointToPointsList(Point p){
    _pointsList.push_back(p);
}
