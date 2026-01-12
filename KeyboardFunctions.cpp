
#include "KeyboardFunctions.h"
#include <cstdlib>
#include <cmath>


void KeyboardFunctions::ProcessNormalKeys(unsigned char key, int x, int y, CameraParameters& params) 
{
	switch (key) {
	case '+':
		params.dist -= 2;	//	apasarea tastelor `-` si `+` schimba pozitia observatorului (se departeaza / aproprie);
		break;
	case '=':
		params.dist += 2;
		break;
	}
	if (key == 27)
		exit(0);
}

void KeyboardFunctions::ProcessSpecialKeys(int key, int xx, int yy, CameraParameters& params)
{
	switch (key)				//	Procesarea tastelor 'LEFT', 'RIGHT', 'UP', 'DOWN';
	{							//	duce la deplasarea observatorului pe suprafata sferica in jurul cubului;
	case GLUT_KEY_LEFT:
		params.beta -= 0.05;
		break;
	case GLUT_KEY_RIGHT:
		params.beta += 0.05;
		break;
	case GLUT_KEY_UP:
		params.alpha += params.incrAlpha1;
		if (abs(params.alpha - params.PI / 2) < 0.05)
		{
			params.incrAlpha1 = 0.f;
		}
		else
		{
			params.incrAlpha1 = 0.05f;
		}
		break;
	case GLUT_KEY_DOWN:
		params.alpha -= params.incrAlpha2;
		if (abs(params.alpha + params.PI / 2) < 0.05)
		{
			params.incrAlpha2 = 0.f;
		}
		else
		{
			params.incrAlpha2 = 0.05f;
		}
		break;
	}
}
