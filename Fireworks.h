#pragma once

#include "RenderedObject.h"
#include "glm/glm.hpp"
#include <vector>

class Fireworks : public RenderedObject {
public:
    struct BezierCurve {
        glm::vec3 p0;    // Start point
        glm::vec3 p1;    // Control point
        glm::vec3 p2;    // End point
        glm::vec3 color; // Curve/particle color
    };
private:
	std::vector<BezierCurve> curves;
    GLuint programId;
	GLuint colorVboId;

    // Uniform locations
    GLint projLocation, viewLocation;
    GLint currentTimeLocation, particleSpeedLocation;
    GLint particleLifetimeLocation, numParticlesLocation;
    GLint tessLevelLocation;

    float currentTime;
public:
	Fireworks();
	void Create() override;
	void setProgramId(GLuint id);
    void AddCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 color);
    void CreateExplosion(glm::vec3 origin, int numCurves, float radius);
    void UpdateVBO();
    void Update(float dt);
    void Render(const glm::mat4& projection, const glm::mat4& view, float particleSpeed, int numParticles, int tessLevel);
    void Reset();
   
};