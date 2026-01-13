#include "Ground.h"

void Ground::Create() {
	// varfurile 
	GLfloat Vertices[] =
	{
		// coordonate                   // culori			// normale					//texturare
		// varfuri "ground"
	   -1500.0f,  -1500.0f, 20.5f, 1.0f,	0.35f, 0.65f, 0.35f,	0.0f, 0.0f, 1.0f,	0.0f,0.0f,
		50.0f,  -1500.0f, 20.5f, 1.0f,		0.35f, 0.65f, 0.35f,	0.0f, 0.0f, 1.0f,	50.0f,0.0f,
		50.0f,  1500.0f,  20.5f, 1.0f,		0.35f, 0.65f, 0.35f,	0.0f, 0.0f, 1.0f,	50.0f,50.0f,
	   -1500.0f,  1500.0f,  20.5f, 1.0f,	0.35f, 0.65f, 0.35f,	0.0f, 0.0f, 1.0f,	0.0f,50.0f,

	   50.0f,  -1500.0f,  40.5f, 1.0f,		0.98f, 0.94f, 0.81f,	0.0f, 0.0f, 1.0f,	25.0f,0.0f,
	   1500.0f,  -1500.0f,  40.5f, 1.0f,	0.98f, 0.94f, 0.81f,	0.0f, 0.0f, 1.0f,	25.0f,5.0f,
	   1500.0f,  1500.0f,	40.5f, 1.0f,	0.98f, 0.94f, 0.81f,	0.0f, 0.0f, 1.0f,	0.0f,5.0f,
	   50.0f,  1500.0f,  40.5f, 1.0f,		0.98f, 0.94f, 0.81f,	0.0f, 0.0f, 1.0f,	0.0f,0.0f,
	};

	// indicii pentru varfuri
	GLubyte Indices[] = {1,2,0,  2,0,3, 5,6,4,  4,6,7};

	glGenVertexArrays(1, &vaoId);
	glGenBuffers(1, &vboId);
	glGenBuffers(1, &eboId);
	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)0);
	// atributul 1 = normale
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
	// atributul 2 = texturare
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(10 * sizeof(GLfloat)));
	// atributul 3 = culoare
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	

}