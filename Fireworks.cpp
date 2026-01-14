#include "Fireworks.h"
#include <vector>
#include <cstdlib>
#include <cmath>

Fireworks::Fireworks() : currentTime(0.0f), totalLifetime(1.0f), isAnimating(false), currentParticleSize(1.0f) {}

void Fireworks::setProgramId(GLuint id) {
	this->programId = id;
}

void Fireworks::Create()
{

    if (programId == 0) {
        std::cout << "ERROR: programId is 0! Call setProgramId first!\n";
        return;
    }

    projLocation = glGetUniformLocation(programId, "projection");
    viewLocation = glGetUniformLocation(programId, "view");
    currentTimeLocation = glGetUniformLocation(programId, "currentTime");
    particleSizeLocation = glGetUniformLocation(programId, "particleSize");
    particleLifetimeLocation = glGetUniformLocation(programId, "particleLifetime");
    tessLevelLocation = glGetUniformLocation(programId, "tessLevel");

    std::cout << "Uniform locations:\n";
    std::cout << "projection: " << projLocation << "\n";
    std::cout << "view: " << viewLocation << "\n";
    std::cout << "currentTime: " << currentTimeLocation << "\n";
    std::cout << "particleSpeed: " << particleSizeLocation << "\n";
	std::cout << "particleLifetime: " << particleLifetimeLocation << "\n";
    std::cout << "tessLevel: " << tessLevelLocation << "\n";

    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // VBO pentru coordonatele punctelor de control + culori
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // actualizat in UpdateVBO
}

void Fireworks::AddCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 color) {
	BezierCurve curve = { p0, p1, p2, color };
	curves.push_back(curve);
}

void Fireworks::CreateExplosion(glm::vec3 origin, int numCurves, float radius, float particleSize, float particleLifetime) {
    
    isAnimating = true;
    totalLifetime = particleLifetime;
    currentParticleSize = particleSize;
    currentTime = 0.0f; 
    curves.clear();

    glm::vec3 fireworksGold = glm::vec3(
        0.94f + 0.05f * (float)rand() / RAND_MAX,
        0.8f + 0.05f * (float)rand() / RAND_MAX,
        0.02f + 0.05f * (float)rand() / RAND_MAX);

	glm::vec3 fireworksRed = glm::vec3(
		0.9f + 0.05f * (float)rand() / RAND_MAX,
		0.1f + 0.05f * (float)rand() / RAND_MAX,
		0.02f + 0.05f * (float)rand() / RAND_MAX);


	for (int i = 0; i < numCurves; ++i) {
		float angle = (2.0f * 3.14159f * i) / numCurves;
        float angle2 = angle + 0.4f * (float)rand() / RAND_MAX;

        glm::vec3 p0 = origin;

        // Punct de control situat "deasupra"
        glm::vec3 p1 = origin + glm::vec3(
            0.7f * radius * cos(angle),
            0.7f * radius * sin(angle),
            -20.0f);

        // Punctul de final 
        glm::vec3 p2 = origin + glm::vec3(
            radius * cos(angle2),
            radius * sin(angle2),
            20.0f);

		int style = rand() % 3;
        switch (style)
        {
        case 0:
            if (numCurves % 2 == 0)
                AddCurve(p0, p1, p2, fireworksGold);
            else
                AddCurve(p0, p1, p2, fireworksRed);
            break;
        case 1:
            AddCurve(p0, p1, p2, fireworksGold);
            break;
        case 2:
            AddCurve(p0, p1, p2, fireworksRed);
            break;
        }
	
	}
	UpdateVBO();
    std::cout << "Total curves: " << curves.size() << "\n";
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

    size_t numPoints = controlPoints.size();
    size_t numColors = colors.size();
    size_t controlPointsSize = numPoints * sizeof(glm::vec3);
    size_t colorsSize = numColors * sizeof(glm::vec3);
    size_t totalSize = controlPointsSize + colorsSize;

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_DYNAMIC_DRAW);

    // Adaugam in buffer coordonatele
    if (numPoints > 0)
        glBufferSubData(GL_ARRAY_BUFFER, 0, controlPointsSize, controlPoints.data());

    // Adaugam culorile incepand de la offsetul controlPointsSize
    if (numColors > 0)
        glBufferSubData(GL_ARRAY_BUFFER, controlPointsSize, colorsSize, colors.data());

    glBindVertexArray(vaoId);

    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)controlPointsSize);

    glBindVertexArray(0);
}

void Fireworks::Update(float dt) {
	if (!isAnimating) return;

    currentTime += dt;

	if (currentTime > totalLifetime) {
		isAnimating = false;
        currentTime = 0.0f;
		//curves.clear();
	}
}

void Fireworks::Render(const glm::mat4& projection, const glm::mat4& view, int tessLevel)
{
    if (!isAnimating || curves.empty()) return; // daca nu avem ce desena -> exit

    glUseProgram(programId);


    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
    glUniform1f(currentTimeLocation, currentTime);
    glUniform1f(particleSizeLocation, currentParticleSize);
    glUniform1f(particleLifetimeLocation, totalLifetime);
    glUniform1i(tessLevelLocation, tessLevel);


    glEnable(GL_BLEND); // amestecarea culorilor
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_PROGRAM_POINT_SIZE);
    glDepthMask(GL_FALSE);


    // setam dimensiunea patch-ului
    // un patch poate fi tratat ca o multime de varfuri pe care va fi aplicat un algoritm de teselare
    // in cazul nostru, un patch este alcatuit din punctele de control ale curbei
    glPatchParameteri(GL_PATCH_VERTICES, 3);

    // desenarea 
    glBindVertexArray(vaoId);

    int numVertices = curves.size() * 3;

    //std::cout << "Drawing " << numVertices << " vertices as patches\n";
    for (size_t i = 0; i < curves.size(); ++i) {
        glDrawArrays(GL_PATCHES, i * 3, 3); // Fiecare patch este alcatui din 3 puncte de control
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL Error: " << err << "\n";
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
	
}