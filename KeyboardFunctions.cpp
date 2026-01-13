
#include "KeyboardFunctions.h"
#include <cstdlib>
#include <cmath>


void KeyboardFunctions::ProcessNormalKeys(unsigned char key, int x, int y, CameraParameters& params, Fireworks& fireworksHandler)
{
	switch (key) {
	case '+':
		params.dist -= 1;	//	apasarea tastelor `-` si `+` schimba pozitia observatorului (se departeaza / aproprie);
		break;
	case '-':
		params.dist += 1;
		break;
	}

	if (key == 'f') {
		// se creeaza o noua explozie de artificii
		float randX = -50.0f + 100.0f * (float)rand() / RAND_MAX;
		float randY = -50.0f + 100.0f * (float)rand() / RAND_MAX;

		float numCurves = 20 + rand() % 30; // intre 20 si 50 de curbe
		float radius = 20.0f + 30.0f * (float)rand() / RAND_MAX; // intre 20 si 50
		float particleSize = 1.0f + 1.0f * (float)rand() / RAND_MAX; // intre 1 si 2

		fireworksHandler.CreateExplosion(glm::vec3(randX, randY, -100.0f), numCurves, radius, particleSize, 2.5f);
		std::cout << "New explosion!";
		glutPostRedisplay();
	}

	if (key == 'c') {
		// se reseteaza animatia
		glutPostRedisplay();
	}

	if (key == 27)
		exit(0);
}

void KeyboardFunctions::ProcessSpecialKeys(int key, int xx, int yy, CameraParameters& params)
{
	switch (key)				//	Procesarea tastelor 'LEFT', 'RIGHT', 'UP', 'DOWN';
	{							//	duce la deplasarea observatorului pe suprafata sferica in jurul cubului;
	case GLUT_KEY_LEFT:
		params.beta -= 0.01;
		break;
	case GLUT_KEY_RIGHT:
		params.beta += 0.01;
		break;
	case GLUT_KEY_F6:
		params.alpha += params.incrAlpha1;
		if (abs(params.alpha - params.PI / 2) < 0.05)
		{
			params.incrAlpha1 = 0.f;
		}
		else
		{
			params.incrAlpha1 = 0.01f;
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
			params.incrAlpha2 = 0.01f;
		}
		break;
	}
}
