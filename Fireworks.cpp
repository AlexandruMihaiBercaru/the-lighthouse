#include "Fireworks.h"
#include <vector>
#include <cstdlib>
#include <cmath>

Fireworks::Fireworks() : currentTime(0.0f) {}

void Fireworks::setProgramId(GLuint id) {
	this->programId = id;
}

void Fireworks::Create()
{
    projLocation = glGetUniformLocation(programId, "projection");
    viewLocation = glGetUniformLocation(programId, "view");
    currentTimeLocation = glGetUniformLocation(programId, "currentTime");
    particleSpeedLocation = glGetUniformLocation(programId, "particleSpeed");
    particleLifetimeLocation = glGetUniformLocation(programId, "particleLifetime");
    numParticlesLocation = glGetUniformLocation(programId, "numParticlesPerCurve");
    tessLevelLocation = glGetUniformLocation(programId, "tessLevel");

    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // VBO pentru coordonatele punctelor de control
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER,
        curves.size() * 3 * sizeof(glm::vec3),
        nullptr,
        GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // VBO pentru culorile curbelor / particulelor
    // VBO pentru culorile curbelor / particulelor
    glGenBuffers(1, &colorVboId);
    glBindBuffer(GL_ARRAY_BUFFER, colorVboId);
    glBufferData(GL_ARRAY_BUFFER,
        curves.size() * 3 * sizeof(glm::vec3),
        nullptr,
        GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Fireworks::AddCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 color) {
	BezierCurve curve = { p0, p1, p2, color };
	curves.push_back(curve);
}

void Fireworks::CreateExplosion(glm::vec3 origin, int numCurves = 20, float radius = 30.0f) {
	for (int i = 0; i < numCurves; ++i) {
		float angle = (2.0f * 3.14159f * i) / numCurves;
        float angle2 = angle + 0.1f;

        glm::vec3 p0 = origin;

        // Punct de control situat "deasupra"
        glm::vec3 p1 = origin + glm::vec3(
            0.6f * radius * cos(angle),
            0.6f * radius * sin(angle),
            25.0f
        );

        // Punctul de final 
        glm::vec3 p2 = origin + glm::vec3(
            radius * cos(angle2),
            radius * sin(angle2),
            -10.0f
        );

        // cularea particulei
        glm::vec3 color = glm::vec3(
            0.5f + 0.5f * (float)rand() / RAND_MAX,
            0.5f + 0.5f * (float)rand() / RAND_MAX,
            0.3f + 0.7f * (float)rand() / RAND_MAX
        );

        AddCurve(p0, p1, p2, color);
	
	}
	UpdateVBO();
}

void Fireworks::UpdateVBO() {
	std::vector<glm::vec3> controlPoints;
	std::vector<glm::vec3> colors;

	for (const auto& curve : curves) {
		controlPoints.push_back(curve.p0);
		controlPoints.push_back(curve.p1);
		controlPoints.push_back(curve.p2);
		
		colors.push_back(curve.color);
		colors.push_back(curve.color);
		colors.push_back(curve.color);
	}

	// Actualizam VBO-ul pentru punctele de control
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(glm::vec3),  &controlPoints[0], GL_DYNAMIC_DRAW);
	// Actualizam VBO-ul pentru culori
	glBindBuffer(GL_ARRAY_BUFFER, colorVboId);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);
}

void Fireworks::Update(float dt) {
    currentTime += dt;
}

void Fireworks::Render(const glm::mat4& projection, const glm::mat4& view,
    float particleSpeed = 0.3f,
    int numParticles = 5,
    int tessLevel = 32)
{

    glUseProgram(programId);


    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
    glUniform1f(currentTimeLocation, currentTime);
    glUniform1f(particleSpeedLocation, particleSpeed);
    glUniform1f(particleLifetimeLocation, 1.0f);
    glUniform1i(numParticlesLocation, numParticles);
    glUniform1i(tessLevelLocation, tessLevel);

    glEnable(GL_BLEND); // amestecarea culorilor
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDepthMask(GL_FALSE);

    // setam dimensiunea patch-ului
    // un patch poate fi tratat ca o multime de varfuri pe care va fi aplicat un algoritm de teselare
    // in cazul nostru, un patch este alcatuit din punctele de control ale curbei
    glPatchParameteri(GL_PATCH_VERTICES, 3);

    // desenarea 
    glBindVertexArray(vaoId);
    glDrawArrays(GL_PATCHES, 0, curves.size() * 3);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

}