#define EDITING_VERTEX 0
#define EDITING_TEXTURE 1
#define EDITING_NORMAL 2

#include <string>
#include <fstream>
#include <iostream>
#include "structs.h"

void countLines(std::string filename, Obj &obj){
	obj.numFaces = 0; obj.numVertex = 0; obj.numNormals = 0;
	std::ifstream file(filename);
	std::string id;
	while(file >> id){
		if(id == "v"){
			obj.numVertex += 1;
		}else if(id == "vn"){
			obj.numNormals += 1;
		}else if(id == "f"){
			obj.numFaces += 1;
		}
	}
	file.close();
}

int countElements(std::string filename, std::string identifier){
	std::ifstream file(filename);
	std::string id = "";
	std::string line;
	int counter = 0;
	while(id != identifier){
		file >> id;
		std::getline(file, line);
	}
	for(int i = 0; line[i + 1] != 0; i++){
		if(line[i] == ' ' && line[i + 1] != ' '){
			counter++;
		}
	}
	file.close();
	return counter;
}

void getVertexPerFace(std::string filename, Obj &obj){
	std::ifstream file(filename);
	std::string id = "";
	std::string line;
	int currentFace = 0;
	while(file >> id){
		std::getline(file, line);
		if(id == "f"){
			int counter = 0;
			for(int i = 0; line[i + 1] != 0; i++){
				if(line[i] == ' ' && line[i + 1] != ' '){
					counter++;
				}
			}
			obj.face[currentFace].vertexPerFace = counter;
			obj.face[currentFace].vertex = new unsigned short int[counter];
			obj.face[currentFace].normal = new unsigned short int[counter];
			currentFace++;
		}
	}
	file.close();
}

void getVertexElements(std::string filename, Obj &obj, std::string identifier){
	if (obj.numNormals == 0 && identifier == "vn") return;
	int numElements = countElements(filename, identifier);
	double maxX = -10000, minX = 10000;
	double maxY = -10000, minY = 10000;
	double maxZ = -10000, minZ = 10000;
	std::string id, x, y, z, w;
	int current = 0;
	std::ifstream file(filename);
	while(file >> id){
		if(id == identifier){
			if(numElements == 3){
				file >> x >> y >> z;
			} else if(numElements == 4){
				file >> x >> y >> z >> w;
			}
			if(identifier == "v"){
				//---x---//
				obj.vertex[current].x = stod(x);
				if(stod(x) > maxX){maxX = stod(x);}
				if(stod(x) < minX){minX = stod(x);}
				//---y---//
				obj.vertex[current].y = stod(y);
				if(stod(y) > maxY){maxY = stod(y);}
				if(stod(y) < minY){minY = stod(y);}
				//---z---//
				obj.vertex[current].z = stod(z);
				if(stod(z) > maxZ){maxZ = stod(z);}
				if(stod(z) < minZ){minZ = stod(z);}
			} else if(identifier == "vn"){
				obj.normal[current].x = stod(x);
				obj.normal[current].y = stod(y);
				obj.normal[current].z = stod(z);
			}
			current++;
		}
	}
	if(identifier == "v"){
		obj.offset.x = minX + ((maxX - minX) / 2);
		obj.offset.y = minY + ((maxY - minY) / 2);
		obj.offset.z = minZ + ((maxZ - minZ) / 2);
	}
	file.close();
}

void getFaceElements(std::string filename, Obj &obj){
	std::ifstream file(filename);
	std::string line;
	std::string id = "";
	int currentVertex = -1;
	int currentFace = 0;
	int numVertexPassed = 0, numNormalsPassed = 0;
	std::string temp;
	while(file >> id){
		std::getline(file, line);
		int editing = EDITING_NORMAL;
		if(id == "v"){numVertexPassed++;}
		if(id == "vn"){numNormalsPassed++;}
		//face identifier
		if(id == "f"){
			for(int i = 1; line[i] != 0; i++){
			//currentFace incrementer and currentVertex resetter
				if(currentVertex == obj.face[currentFace].vertexPerFace){
					currentFace++;
					currentVertex = 0;
				}
				//element identifier
				if(line[i - 1] == ' ' && line[i] != ' '){
					editing = EDITING_VERTEX;
					temp = "";
					currentVertex++;
				} else if(editing == EDITING_VERTEX && line[i - 1] == '/'){
					editing = EDITING_TEXTURE;
					temp = "";
				} else if(editing == EDITING_TEXTURE && line[i - 1] == '/'){
					editing = EDITING_NORMAL;
					temp = "";
				}
				//element "builder" (if isn't '/' or ' ')
				if(!(line[i] == '/' || line[i] == ' ')){
					temp += line[i];
				//element applier (if is '/' or ' ' and has something in temp)
				} else if(temp != ""){
					if(editing == EDITING_VERTEX){
						obj.face[currentFace].vertex[currentVertex] = (stoi(temp) > 0) ? abs(stoi(temp)) - 1 : numVertexPassed - abs(stoi(temp));
					} else if(editing == EDITING_TEXTURE){
						obj.face[currentFace].texture = (stoi(temp) > 0) ? abs(stoi(temp)) - 1 : obj.numVertex - abs(stoi(temp));
					} else if(editing == EDITING_NORMAL){
						obj.face[currentFace].normal[currentVertex] = (stoi(temp) > 0) ? abs(stoi(temp)) - 1 : numNormalsPassed - abs(stoi(temp));
					}
				}
			}
			//---last normal from current line---//
			obj.face[currentFace].normal[currentVertex] = (stoi(temp) > 0) ? abs(stoi(temp)) - 1 : numNormalsPassed - abs(stoi(temp));
		}
	}
}

void loadObj(Obj& obj, std::string filename){
	countLines(filename, obj);
	obj.vertex = new Point3D[obj.numVertex];
	if(obj.numNormals > 0){ obj.normal = new Point3D[obj.numNormals]; }
	obj.face = new Face[obj.numFaces];
	std::cout << "Loading vertexes...\n";
	getVertexElements(filename, obj, "v");
	std::cout << "Loaded " << obj.numVertex << " vertex!\n\n";
	std::cout << "Loading normals...\n";
	if(obj.numNormals > 0){ getVertexElements(filename, obj, "vn"); }
	std::cout << "Loaded " << obj.numNormals << " normals!\n\n";
	std::cout << "Loading faces...\n";
	getVertexPerFace(filename, obj);
	getFaceElements(filename, obj);
	std::cout << "Loaded " << obj.numFaces << " faces!\n\n";
}