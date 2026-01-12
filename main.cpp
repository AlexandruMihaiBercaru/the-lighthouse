#include <windows.h>  
#include <stdio.h>
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <cstdlib> 
#include <vector>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include "loadShaders.h"
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "objloader.hpp"  // pentru citirea din fisierul .obj
#include "Parameters.h"
#include "KeyboardFunctions.h"
#include "Ground.h"
#include "Sfera.h"
#include "Cone.h"

#include "Fireworks.h"

//  Identificatorii obiectelor de tip OpenGL;
GLuint
VaoId, VboId, // pt modelul importat (daca folosim objLoader)
ProgramId,
nrVertLocation,
myMatrixLocation,
viewPosLocation,
viewLocation,
objectLocation,
codColLocation,
lightPosLocation,
matrUmbraLocation,
projLocation,
fireworksProgramId;

// Valoarea lui pi
float PI = 3.141592;

// pt modelul importat - varianta objLoader
int nrVertices, codCol;
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs; //texturare
std::vector<glm::vec3> normals;

// Matrice utilizate
glm::mat4 myMatrix, mat;
glm::mat4 view;
glm::mat4 projection;

glm::mat4 scaleSphere, translateSphere, modelMatrix,
scaleCone, rotateCone, translateCone,
scaleMatrix, rotateMatrix, translateMatrix;

//	Elemente pentru matricea de vizualizare;
float refX = 0.0f, refY = 0.0f, refZ = 0.0f,
obsX, obsY, obsZ,
vX = 0.0f, vY = 0.0f, vZ = 1.0f;

//	Elemente pentru matricea de proiectie;
float width = 800, height = 600, dNear = 3.f, fov = 30;

// sursa de lumina
float xL = 500.f, yL = 100.f, zL = 200.f;
// matricea umbrei
float matrUmbra[4][4];

Fireworks fireworks;
CameraParameters cameraParams;
Ground ground;
int NR_PARR_SFERA = 10, NR_MERID_SFERA = 20;
float radius = 20;
Sfera sfera(NR_PARR_SFERA, NR_MERID_SFERA, radius);
int NR_PARR_CON = 18, NR_MERID_CON = 12;
Cone con(NR_PARR_CON, NR_MERID_CON);

//	Identificatori optiuni meniu;
enum {
	FOG, NO_FOG
};
int renderMode;

//	Meniu pentru selectia optiunilor;
void Menu(int selection)
{
	renderMode = selection;
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y){
	KeyboardFunctions::ProcessNormalKeys(key, x, y, cameraParams);
}

void processSpecialKeys(int key, int xx, int yy){
	KeyboardFunctions::ProcessSpecialKeys(key, xx, yy, cameraParams);
}

// Se initializeaza un vertex Buffer Object(VBO) pentru transferul datelor spre memoria placii grafice(spre shadere);
// In acesta se stocheaza date despre varfuri;
void CreateVBO(void) // pentru modele importate - folsind fisierele objLoader
{
	// Generare VAO;
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	// Generare VBO - varfurile si normalele sunt memorate in sub-buffere;
	glGenBuffers(1, &VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) + normals.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), normals.size() * sizeof(glm::vec3), &normals[0]);

	// Atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = normale
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(vertices.size() * sizeof(glm::vec3)));
}


//  Eliminarea obiectelor de tip VBO dupa rulare;
void DestroyVBO(void)
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId); 
}

//  Crearea si compilarea obiectelor de tip shader;
void CreateShaders(void)
{
	ProgramId = LoadShaders("10_Sol_Shader.vert", "10_Sol_Shader.frag");
	glUseProgram(ProgramId);

	fireworksProgramId = LoadShadersTessGeom("Bezier_Shader.vert", "Bezier_Shader.tcs", "Bezier_Shader.tes", "Bezier_Shader.geom", "Bezier_Shader.frag");
	fireworks.setProgramId(fireworksProgramId);

}

// Elimina obiectele de tip shader dupa rulare;
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

//  Functia de eliberare a resurselor alocate de program;
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
};

//  Setarea parametrilor necesari pentru fereastra de vizualizare;
void Initialize(void)
{
	glClearColor(0.57f, 0.57f, 0.50f, 1.0f); // culoarea de fond a ecranului

	// Incarcarea modelului 3D in format OBJ, trebuie sa fie in acelasi director cu proiectul actual;
	//bool model = loadOBJ("12151_Christmas_Tree_l1.obj", vertices, uvs, normals);
	nrVertices = vertices.size();

	// Crearea VBO / shadere-lor
	//CreateVBO(); // pt model importat

	ground.Create();
	sfera.Create();
	con.Create();

	CreateShaders();

	// Locatii ptr shader
	nrVertLocation = glGetUniformLocation(ProgramId, "nrVertices");
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	viewPosLocation = glGetUniformLocation(ProgramId, "viewPos");
	viewLocation = glGetUniformLocation(ProgramId, "view");
	projLocation = glGetUniformLocation(ProgramId, "projection");
	objectLocation = glGetUniformLocation(ProgramId, "objectId");
	//lightColorLocation = glGetUniformLocation(ProgramId, "lightColor");
	lightPosLocation = glGetUniformLocation(ProgramId, "lightPos");
	matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
	codColLocation = glGetUniformLocation(ProgramId, "codCol");

	// Variabile ce pot fi transmise catre shader
	glUniform1i(nrVertLocation, nrVertices);
}

void SetMVP(void)
{
	//pozitia observatorului -> transmisa catre shader (vom avea nevoie)
	obsX = refX + cameraParams.dist * cos(cameraParams.alpha) * cos(cameraParams.beta);
	obsY = refY + cameraParams.dist * cos(cameraParams.alpha) * sin(cameraParams.beta);
	obsZ = refZ + cameraParams.dist * sin(cameraParams.alpha);
	glUniform3fv(viewPosLocation, 1, glm::value_ptr(glm::vec3(obsX, obsY, obsZ)));

	// reperul de vizualizare
	glm::vec3 Obs = glm::vec3(obsX, obsY, obsZ);   // se schimba pozitia observatorului	
	glm::vec3 PctRef = glm::vec3(refX, refY, refZ); // pozitia punctului de referinta
	glm::vec3 Vert = glm::vec3(vX, vY, vZ); // verticala din planul de vizualizare 

	// matricea de vizualizare -> transmisa catre shader
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// matricea de proiectie -> transmisa catre shader 
	projection = glm::infinitePerspective(GLfloat(fov), GLfloat(width) / GLfloat(height), dNear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
}

//	Functia de desenare a graficii pe ecran;
void RenderFunction(void)
{
	// Initializare ecran + test de adancime;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	SetMVP();

	// Matricea de modelare - aplicam rotatiile cand importam modele, ca sa le pun "in picioare"
	myMatrix = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0.0, 1.0, 0.0))
		* glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0.0, 0.0, 1.0));

	//myMatrix = glm::mat4(1.0f);
	//glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// matricea pentru umbra
	float D = -20.0f;
	//xL = obsX; yL = obsY; zL = obsZ;
	matrUmbra[0][0] = zL + D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3] = 0;
	matrUmbra[1][0] = 0; matrUmbra[1][1] = zL + D; matrUmbra[1][2] = 0; matrUmbra[1][3] = 0;
	matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] = -1;
	matrUmbra[3][0] = -D * xL; matrUmbra[3][1] = -D * yL; matrUmbra[3][2] = -D * zL; matrUmbra[3][3] = zL;

	glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);
	glUniform3f(lightPosLocation, obsX, obsY, obsZ);
	glUniform1i(codColLocation, 0);

	// optiune efect de ceata sau nu -> extindem ulterior pentru setari mai complexe
	switch (renderMode) {
		case NO_FOG:
			glUniform1i(glGetUniformLocation(ProgramId, "fogEnable"), 0);
			break;
		case FOG:
			glUniform1i(glGetUniformLocation(ProgramId, "fogEnable"), 1);
			break;
	}


	// desenare ground
	glBindVertexArray(ground.vaoId);
	mat = glm::mat4(1.0f);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &mat[0][0]);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, 0);


	glUniform1i(objectLocation, 0); // 0 - generat procedural

	// SFERA 1 - in centru (nu modific matricea)
	glBindVertexArray(sfera.vaoId);
	modelMatrix = myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);

	// SFERA 2 - micsorata si translatata deasupra primei sfere
	scaleSphere = glm::scale(glm::mat4(1.0f), glm::vec3(0.7, 0.7, 0.7));
	translateSphere = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -28.0));
	modelMatrix = translateSphere * scaleSphere * myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);

	// SFERA 3 - micsorata si translatata (si mai mult)
	scaleSphere = glm::scale(glm::mat4(1.0f), glm::vec3(0.4, 0.4, 0.4));
	translateSphere = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -48.0));
	modelMatrix = translateSphere * scaleSphere * myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);

	glUniform1i(objectLocation, 2);
	scaleSphere = glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05));
	translateSphere = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0, 6.0, -52.0));
	modelMatrix = translateSphere * scaleSphere * myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);

	translateSphere = glm::translate(glm::mat4(1.0f), glm::vec3(4.0, 6.0, -52.0));
	modelMatrix = translateSphere * scaleSphere * myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);

	glUniform1i(objectLocation, 0);
	scaleCone = glm::scale(glm::mat4(1.0f), glm::vec3(0.4, 0.4, 1.0));
	translateCone = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 12.0, -50.0));
	rotateCone = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = translateCone * rotateCone * scaleCone;
	
	glBindVertexArray(con.vaoId);
	con.Render(myMatrixLocation, modelMatrix);

	// --------------- DESENARE UMBRE ----------------------------
	glUniform1i(codColLocation, 1);

	glUniform1i(objectLocation, 0); // 0 - generat procedural

	// SFERA 1 - in centru (nu modific matricea)
	glBindVertexArray(sfera.vaoId);
	modelMatrix = myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);

	// SFERA 2 - micsorata si translatata deasupra primei sfere
	scaleSphere = glm::scale(glm::mat4(1.0f), glm::vec3(0.7, 0.7, 0.7));
	translateSphere = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -28.0));
	modelMatrix = translateSphere * scaleSphere * myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);

	// SFERA 3 - micsorata si translatata (si mai mult)
	scaleSphere = glm::scale(glm::mat4(1.0f), glm::vec3(0.4, 0.4, 0.4));
	translateSphere = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -48.0));
	modelMatrix = translateSphere * scaleSphere * myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);


	glUniform1i(objectLocation, 2);
	scaleSphere = glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05));
	translateSphere = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0, 6.0, -52.0));
	modelMatrix = translateSphere * scaleSphere * myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);

	translateSphere = glm::translate(glm::mat4(1.0f), glm::vec3(4.0, 6.0, -52.0));
	modelMatrix = translateSphere * scaleSphere * myMatrix;
	sfera.Render(myMatrixLocation, modelMatrix);

	glUniform1i(objectLocation, 0);
	scaleCone = glm::scale(glm::mat4(1.0f), glm::vec3(0.4, 0.4, 1.0));
	translateCone = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 12.0, -50.0));
	rotateCone = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = translateCone * rotateCone * scaleCone;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	// CONUL 
	glBindVertexArray(con.vaoId);
	con.Render(myMatrixLocation, modelMatrix);

	glutSwapBuffers();
	glFlush();
}

//	Punctul de intrare in program, se ruleaza rutina OpenGL;
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("Utilizarea unui model predefinit in format OBJ");
	glewInit();
	Initialize();
	glutIdleFunc(RenderFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);

	//	Initializarea meniului - activat prin click dreapta in aplicatie;
	glutCreateMenu(Menu);
	glutAddMenuEntry("No fog", NO_FOG);
	glutAddMenuEntry("Fog", FOG);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}

