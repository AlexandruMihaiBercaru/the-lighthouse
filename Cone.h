#pragma once

#include "RenderedObject.h"
#include "glm/glm.hpp"

class Cone : public RenderedObject {
public:
	int nrVf, nrParr, nrMerid;
	float PI;
	Cone(int nrParr, int nrMerid);
	void Create() override;
	void Render(GLuint matLoc, glm::mat4 modelMatrix);
};
