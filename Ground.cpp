#include "Ground.h"

void Ground::Create() {
	// varfurile 
	GLfloat Vertices[] =
	{
		// coordonate                   // culori			// normale
		// varfuri "ground"
	   -1500.0f,  -1500.0f, 20.5f, 1.0f,  1.0f, 1.0f, 0.9f,  0.0f, 0.0f, 1.0f,
		1500.0f,  -1500.0f, 20.5f, 1.0f,  1.0f, 1.0f, 0.9f,  0.0f, 0.0f, 1.0f,
		1500.0f,  1500.0f,  20.5f, 1.0f,  1.0f, 1.0f, 0.9f,  0.0f, 0.0f, 1.0f,
	   -1500.0f,  1500.0f,  20.5f, 1.0f,  1.0f, 1.0f, 0.9f,  0.0f, 0.0f, 1.0f,
	};

	// indicii pentru varfuri
	GLubyte Indices[] = {1, 2, 0,   2, 0, 3};

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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
	// atributul 1 = normale
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
	// atributul 2 = culoare
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

}