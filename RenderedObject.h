#pragma once
#include <GL/glew.h>

class RenderedObject {
public:
    GLuint vaoId = 0;
    GLuint vboId = 0;
    GLuint eboId = 0;

    virtual ~RenderedObject();
    virtual void Create() = 0;
    virtual void Destroy();
};