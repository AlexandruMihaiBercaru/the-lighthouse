#include "Dock.h"




 


Dock::Dock(float length, float width, float height, int nrSegmente):cil(Cilindru(3, 10, 2.0f, 30.0f, glm::vec3(0.55, 0.27, 0.07))) {
	this->length = length;
	this->width = width;
	this->height = height;
	this->nrSegmente = nrSegmente;
}


/// <summary>
/// Important	
/// z = 20.5f = ground
/// z = 40.5f = water 
/// </summary>
void Dock::Create() {
	// Matricele pentru varfuri, culori, indici
	const float ground = 20.5f;
	const int nrVf = 8;
	std::vector<glm::vec4> Vertices(nrVf);
	std::vector<glm::vec3> Colors(nrVf);
	std::vector<glm::vec3> Normals(nrVf);
	std::vector<GLushort> Indices(36);

	Vertices[0] = glm::vec4(0,			 0, ground, 1.0f);
	Vertices[1] = glm::vec4(length,		 0, ground, 1.0f);
	Vertices[2] = glm::vec4(length,  width, ground, 1.0f);
	Vertices[3] = glm::vec4(0,		 width, ground, 1.0f);


	Vertices[4] = glm::vec4(0,			 0, ground + height , 1.0f);
	Vertices[5] = glm::vec4(length,		 0, ground + height , 1.0f);
	Vertices[6] = glm::vec4(length,  width, ground + height , 1.0f);
	Vertices[7] = glm::vec4(0,		 width, ground + height , 1.0f);




	for (int i = 0; i < nrVf; i++) {
		Colors[i] = glm::vec4(0.55, 0.27, 0.07, 1.0);
		Normals[i] = glm::vec3(0.8f,0.8f,1.0f);
	}

	//long live ms paint
	Indices = { 0,1,2, 2,3,0,
				0,3,7, 7,4,0,
				0,1,4, 4,5,1,
				1,5,6, 6,1,2,
				2,3,7, 7,2,6,
				6,7,4, 4,5,6
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	glGenBuffers(1, &vboId); // atribute
	glGenBuffers(1, &eboId); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec4) + Normals.size() * sizeof(glm::vec3) + Colors.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, Vertices.size() * sizeof(glm::vec4), Vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec4), Normals.size() * sizeof(glm::vec3), Normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec4) + Normals.size() * sizeof(glm::vec3), Colors.size() * sizeof(glm::vec3), Colors.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLushort), Indices.data(), GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = normala
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(Vertices.size() * sizeof(glm::vec4)));
	glEnableVertexAttribArray(2); // atributul 2 = culoare
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(Vertices.size() * sizeof(glm::vec4) + Normals.size() * sizeof(glm::vec3)));

	cil.Create();
}

void Dock::RenderBlock() {
	for (int i = 0; i < 6 * 2; i++) {
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)((3 * i) * sizeof(GLushort)));
	}




}

void Dock::Render(GLuint matLoc, glm::mat4 modelMatrix) {
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
	int side = -1;

	glm::mat4 orig = modelMatrix;



	for (int i =0; i<nrSegmente;i++)
	{
		if (i > 0) {
			modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->length, 0.0f, 0.0f)) * modelMatrix;
			glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
		}
		RenderBlock();
		if (i>3 && i % 3 == 2) {
			for (int j = 0; j < 2; j++) {
				modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, this->width * side, 0.0f)) * modelMatrix;
				glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
				RenderBlock();
			}
			modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2 * this->width * side , 0.0f)) * modelMatrix;
			glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
			side *= -1;
		}
	}

	modelMatrix = orig;
	glBindVertexArray(cil.vaoId);

	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->length, 0, 17.5f)) * modelMatrix;
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
	cil.Render(matLoc, modelMatrix);

	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, this->width, 0.0f))* modelMatrix;
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
	cil.Render(matLoc, modelMatrix);


	side = -1;


	for (int i = 1; i < nrSegmente; i++)
	{
		
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->length, -1* this->width, 0.0f)) * modelMatrix;
		glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
		
		cil.Render(matLoc, modelMatrix);
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,this->width, 0.0f)) * modelMatrix;
		glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);

		cil.Render(matLoc, modelMatrix);




		if (i > 3 && i % 3 == 2 ){
			if (side == -1) {
				modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, - this->width, 0.0f)) * modelMatrix;
			}



			for (int j = 0; j < 2; j++) {
				modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, this->width*side, 0.0f)) * modelMatrix;
				glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
				cil.Render(matLoc, modelMatrix);
				modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-this->length, 0.0f, 0.0f)) * modelMatrix;
				glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
				cil.Render(matLoc, modelMatrix);
				modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->length, 0.0f, 0.0f)) * modelMatrix;

			}
		
			if (side == -1) {
				modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,  this->width , 0.0f)) * modelMatrix;
			}

			modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2 * this->width * side, 0.0f)) * modelMatrix;
			glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
			side *= -1;
		}

	}

}

