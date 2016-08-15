/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeometricObject.h
 * Author: renato
 *
 * Created on 14 de Agosto de 2016, 12:06
 */

#ifndef GEOMETRICOBJECT_H
#define GEOMETRICOBJECT_H

#include <string>   //std::string
#include <list>     //sdd::list
#include "Point.h"

class GeometricObject {
public:
    GeometricObject(std::string name);
    GeometricObject();
    virtual ~GeometricObject();
    std::list<Point> getPointsList();
    void addPointToPointsList(Point p);
    std::string getName();
private:
    std::string _name;
    std::list<Point> _pointsList;
};

#endif /* GEOMETRICOBJECT_H */

