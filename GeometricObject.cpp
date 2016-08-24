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

GeometricObject::GeometricObject(std::string name, Point p) {
    _name = name;
    _pointsVector.push_back(p);
}

GeometricObject::GeometricObject(std::string name, std::vector<Point> points) {
    _name = name;
    _pointsVector = points;
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

int GeometricObject::getNumPoints() {
    return _pointsVector.size();
}

Point GeometricObject::getMassCenter() {
    double x = 0;
    double y = 0;
    for (int i = 0; i < _pointsVector.size(); i++) {
        x = x + _pointsVector[i].getX();
        y = y + _pointsVector[i].getY();
    }
    x = x / _pointsVector.size();
    y = y / _pointsVector.size();

    return Point(x, y);

}

void GeometricObject::prepareTranslateMatrix(double dx, double dy) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (i == j) {
                _matrix[i][j] = 1;
            } else {
                _matrix[i][j] = 0;
            }
        }
    }
    _matrix [3][1] = dx;
    _matrix [3][2] = dy;
}

void GeometricObject::prepareEscalonateMatrix(double sx, double sy) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (i == j) {
                _matrix[i][j] = 1;
            } else {
                _matrix[i][j] = 0;
            }
        }
    }
    _matrix [1][1] = sx;
    _matrix [2][2] = sy;
}

void GeometricObject::prepareRotateMatrix(double angle) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (i == j) {
                _matrix[i][j] = 1;
            } else {
                _matrix[i][j] = 0;
            }
        }
    }
    _matrix [1][1] = cos(angle);
    _matrix [1][2] = -sin(angle);
    _matrix [2][1] = sin(angle);
    _matrix [2][2] = cos(angle);
}

void GeometricObject::translateObject() {


}

void GeometricObject::escalonateObject() {
}

void GeometricObject::rotateObject(Point p) {
}
