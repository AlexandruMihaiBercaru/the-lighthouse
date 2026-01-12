#pragma once

#include "RenderedObject.h"
#include "glm/glm.hpp"

class Sfera : public RenderedObject {
public:
	int nrVf, nrParr, nrMerid;
	float radius;
	float PI;
	Sfera(int nrParr, int nrMerid, float radius);
	void Create() override;
	void Render(GLuint matLoc, glm::mat4 modelMatrix);
};