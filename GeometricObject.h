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
#include <vector>   //std::vector
#include <cmath>
#include "Point.h"
#include "Matrix.h"


static const int MATRIX_SIZE = 3;

class GeometricObject {
public:
    GeometricObject(std::string name);
    GeometricObject(std::string name, Point p);
    GeometricObject(std::string name, std::vector<Point> points);
    GeometricObject();
    virtual ~GeometricObject();
    std::vector<Point> getPointsVector();
    void addPointToPointsVector(Point p);
    std::string getName();
    Point getMassCenter();
    void prepareMatrix();
    void prepareTranslateMatrix(double dx, double dy);
    void prepareEscalonateMatrix(double sx, double sy);
    void prepareRotateMatrix(double angle);
    void translateObject();
    void escalonateObject();
    void rotateObject(Point p);
private:
    std::string _name;
    std::vector<Point>_pointsVector;
    double _matrix[MATRIX_SIZE][MATRIX_SIZE];
};

#endif /* GEOMETRICOBJECT_H */  

