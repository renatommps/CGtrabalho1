/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DisplayFile.h
 * Author: renato
 *
 * Created on 15 de Agosto de 2016, 12:50
 */

#ifndef DISPLAYFILE_H
#define DISPLAYFILE_H

#include <vector>       //std::vector
#include "GeometricObject.h"

class DisplayFile {
public:
    DisplayFile(GeometricObject obj);
    DisplayFile();
    virtual ~DisplayFile();
    void addObject(GeometricObject obj);
    std::vector<GeometricObject> getObjects();
private:
    std::vector<GeometricObject> ObjectsList;
};

#endif /* DISPLAYFILE_H */

