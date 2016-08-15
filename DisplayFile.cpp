/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DisplayFile.cpp
 * Author: renato
 * 
 * Created on 15 de Agosto de 2016, 12:50
 */

#include "DisplayFile.h"

DisplayFile::DisplayFile(GeometricObject obj) {
    ObjectsList.push_back(obj);
}

DisplayFile::DisplayFile() {
}

DisplayFile::~DisplayFile() {
}

void DisplayFile::addObject(GeometricObject obj) {
    ObjectsList.push_back(obj);
}

std::list<GeometricObject> DisplayFile::getObjects() {
    return ObjectsList;
}