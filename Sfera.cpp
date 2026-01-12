#include "Sfera.h"
#include <vector>
#include <cmath>
#include <cstdlib>


Sfera::Sfera(int nrParr, int nrMerid, float radius):
	nrParr(nrParr), nrMerid(nrMerid), radius(radius), PI(3.141592f)
{
	nrVf = (nrParr + 1) * (nrMerid + 1);
}


void Sfera::Create()
{
	// Matricele pentru varfuri, culori, indici

	std::vector<glm::vec4> Vertices(nrVf);
	std::vector<glm::vec3> Colors(nrVf);
	std::vector<glm::vec3> Normals(nrVf);
	std::vector<GLushort> Indices(4 * nrVf);

	float U_MIN = -PI / 2, U_MAX = PI / 2, V_MIN = 0, V_MAX = 2 * PI;
	float step_u = (U_MAX - U_MIN) / nrParr, step_v = (V_MAX - V_MIN) / nrMerid;
	float u, v, x_vf, y_vf, z_vf;
	int index, index1, index2, index3, index4;

	for (int merid = 0; merid < nrMerid + 1; merid++)
	{
		for (int parr = 0; parr < nrParr + 1; parr++)
		{
			// implementarea reprezentarii parametrice 
			u = U_MIN + parr * step_u; // valori pentru u si v
			v = V_MIN + merid * step_v;
			x_vf = radius * cosf(u) * cosf(v); // coordonatele varfului corespunzator lui (u,v)
			y_vf = radius * cosf(u) * sinf(v);
			z_vf = radius * sinf(u) + 0.05 * radius * rand() / RAND_MAX;;

			// identificator ptr varf; coordonate + culoare + indice la parcurgerea meridianelor
			index = merid * (nrParr + 1) + parr;
			Vertices[index] = glm::vec4(x_vf, y_vf, z_vf, 1.0);
			Colors[index] = glm::vec3(0.92f, 0.92f, 0.93f);
			Normals[index] = glm::vec3(cosf(u) * cosf(v), cosf(u) * sinf(v), sinf(u));

			// indicii pentru desenarea fetelor, pentru varful curent sunt definite 4 varfuri
			if ((parr + 1) % (nrParr + 1) != 0) // varful considerat sa nu fie Polul Nord
			{
				index1 = index; // varful v considerat
				index2 = index + (nrParr + 1); // dreapta lui v, pe meridianul urmator
				index3 = index2 + 1;  // dreapta sus fata de v
				index4 = index + 1;  // deasupra lui v, pe acelasi meridian
				if (merid == nrMerid)  // la ultimul meridian, patrulaterul este ignorat
				{
					index2 = index;
					index3 = index;
					index4 = index;
				}
				// stocare indici pentru fata in matricea de indici
				Indices[4 * index] = index1;
				Indices[4 * index + 1] = index2;
				Indices[4 * index + 2] = index3;
				Indices[4 * index + 3] = index4;
			}

		}
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
}


void Sfera::Render(GLuint matLoc, glm::mat4 modelMatrix)
{
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &modelMatrix[0][0]);
	int NR_VF = (nrParr + 1) * (nrMerid + 1);
	for (int patr = 0; patr < NR_VF; patr++)
	{
		if ((patr + 1) % (nrParr + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*)((4 * patr) * sizeof(GLushort)));
	}
}