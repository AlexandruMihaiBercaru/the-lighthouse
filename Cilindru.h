#pragma once
#include "RenderedObject.h"
#include "glm/glm.hpp"

class Cilindru : public RenderedObject
{
	glm::vec3 Color;
	public:
	int nrParr, nrMerid,nrVf;
	float radius, height;
	float PI;
	Cilindru(int nrParr, int nrMerid, float radius, float height, glm::vec3 color = glm::vec3(0.2f, 0.2f, 0.2f));
	void Create() override;
	void Render(GLuint matLoc, glm::mat4 modelMatrix);
};

