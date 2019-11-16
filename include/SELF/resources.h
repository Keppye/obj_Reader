#ifndef RESOURCES_H
#define RESOURCES_H

#include <iostream>
#include "structs.h"

int countLines(std::string filename, std::string identifier);
int countElements(std::string filename, std::string identifier);
void getVertexPerFace(std::string filename, Obj &obj);
void getVertexElements(std::string filename, Point3D* array, std::string identifier);
void getFaceElements(std::string filename, Obj &obj);


#endif // RESOURCES_H