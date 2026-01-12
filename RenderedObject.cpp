#include "RenderedObject.h"


// destructorul
RenderedObject::~RenderedObject() {
	Destroy();
}

void RenderedObject::Destroy() {
	if(vaoId)
		glDeleteVertexArrays(1, &vaoId);
	if(vboId)
		glDeleteBuffers(1, &vboId);
	if(eboId)
		glDeleteBuffers(1, &eboId);
}

