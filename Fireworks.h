#pragma once

#include "RenderedObject.h"
#include "glm/glm.hpp"
#include <vector>
#include <iostream>

class Fireworks : public RenderedObject {
public:
    struct BezierCurve {
        glm::vec3 p0;    
        glm::vec3 p1;    
        glm::vec3 p2;    
        glm::vec3 color; 
    };
private:
	std::vector<BezierCurve> curves;

    GLuint programId;
	GLuint colorVboId;

    GLint projLocation, viewLocation,
     currentTimeLocation, particleSizeLocation,
     particleLifetimeLocation, numParticlesLocation,
     tessLevelLocation;

    float currentTime, totalLifetime, currentParticleSize;
    bool isAnimating;

public:
	Fireworks();
	void Create() override;
	void setProgramId(GLuint id);
    void AddCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 color);
    void CreateExplosion(glm::vec3 origin, int numCurves, float radius, float particleSize, float particleLifetime);
    void UpdateVBO();
    void Update(float dt);
    void Render(const glm::mat4& projection, const glm::mat4& view, int tessLevel);
    //void ResetAnimation();
   
};