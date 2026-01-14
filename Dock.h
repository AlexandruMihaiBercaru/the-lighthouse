#pragma once
#include "RenderedObject.h"
#include "glm/glm.hpp"
#include <vector>
#include <cmath>
#include <cstdlib>
#include "glm/gtc/matrix_transform.hpp"
#include "Cilindru.h"

class Dock:public RenderedObject
{
	int nrSegmente;
	float length, width, height;
	void RenderBlock();
	Cilindru cil;
public:
	Dock(float length, float width, float height,int nrSegmente);
	void Create() override;
	void Render(GLuint matLoc, glm::mat4 modelMatrix);
};

