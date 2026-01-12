#pragma once
#include "Parameters.h"
#include <GL/glew.h> 
#include <GL/freeglut.h> 

class KeyboardFunctions {
public:
	static void ProcessNormalKeys(unsigned char key, int x, int y, CameraParameters &params);
	static void ProcessSpecialKeys(int key, int xx, int yy, CameraParameters &params);

}
;