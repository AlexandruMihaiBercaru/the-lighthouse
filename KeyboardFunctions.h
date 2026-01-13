#pragma once
#include "Parameters.h"
#include "Fireworks.h"
#include <GL/glew.h> 
#include <GL/freeglut.h> 
#include <iostream>

class KeyboardFunctions {
public:
	static void ProcessNormalKeys(unsigned char key, int x, int y, CameraParameters &params, Fireworks& fireworksHandler);
	static void ProcessSpecialKeys(int key, int xx, int yy, CameraParameters &params);

}
;