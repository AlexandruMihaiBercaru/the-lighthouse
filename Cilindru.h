#pragma once
#include "RenderedObject.h"
#include "glm/glm.hpp"

class Cilindru : public RenderedObject
{
	public:
	int nrParr, nrMerid,nrVf;
	float radius, height;
	float PI;
	Cilindru(int nrParr, int nrMerid, float radius, float height);
	void Create() override;
	void Render(GLuint matLoc, glm::mat4 modelMatrix);
};

