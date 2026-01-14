#include "Spotlight.h"
#include "glm/gtc/type_ptr.hpp"
#include <cmath>

Spotlight::Spotlight(glm::vec3 pos, float cutoff, float outerCutoff, float speed)
    : position(pos),
    cutoffAngle(cutoff),
    outerCutoffAngle(outerCutoff),
    rotationSpeed(speed),
    baseDirection(1.0f, 0.0f, -3.0f),  // Directie initiala (spre exterior si putin in jos)
    orientation(1.0f, 0.0f, 0.0f, 0.0f) // Cuaternion identitate
{
    // Normalizare directie de baza
    baseDirection = glm::normalize(baseDirection);
    direction = baseDirection;

    // Culori pentru spotlight (lumina alba puternica)
    ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    diffuseColor = glm::vec3(1.0f, 1.0f, 0.9f);
    specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

    // Factori de atenuare (pentru un far puternic)
    constant = 1.0f;
    linear = 0.0014f;
    quadratic = 0.000007f;

    //Nou
    renderLightSource = true;
    lightSourceRadius = 3.0f;
    isRotating = false;           // Iniial static
}

//void Spotlight::Update(float deltaTime) {
//    // Calculam unghiul de rotatie pentru acest frame
//    float angle = rotationSpeed * deltaTime;
//
//    // Cream un cuaternion de rotatie in jurul axei Z (verticala)
//    // Pentru rotatie in jurul axei Z: axa = (0, 0, 1)
//    glm::vec3 axis(0.0f, 0.0f, 1.0f);
//    glm::quat rotationQuat = glm::angleAxis(angle, axis);
//
//    // Compunem rotatia curenta cu noua rotatie
//    orientation = rotationQuat * orientation;
//
//    // Normalizam cuaternionul pentru a evita acumularea erorilor numerice
//    orientation = glm::normalize(orientation);
//
//    // Aplicam rotatia la directia de baza pentru a obtine directia curenta
//    // In GLM, folosim functia rotate
//    direction = glm::rotate(orientation, baseDirection);
//    direction = glm::normalize(direction);
//}

//Nou
void Spotlight::Update(float deltaTime) {
    // Doar daca este activa rotatia
    if (!isRotating) return;

    // Calculam unghiul de rotatie pentru acest frame
    float angle = rotationSpeed * deltaTime;

    // Cream un cuaternion de rotatie in jurul axei Z (verticala)
    glm::vec3 axis(0.0f, 0.0f, 1.0f);
    glm::quat rotationQuat = glm::angleAxis(angle, axis);

    // Compunem rotatia curenta cu noua rotatie
    orientation = rotationQuat * orientation;

    // Normalizam cuaternionul pentru a evita acumularea erorilor numerice
    orientation = glm::normalize(orientation);

    // Aplicam rotatia la directia de baza pentru a obtine directia curenta
    direction = glm::rotate(orientation, baseDirection);
    direction = glm::normalize(direction);
}

void Spotlight::SetPosition(glm::vec3 pos) {
    position = pos;
}

void Spotlight::SetRotationSpeed(float speed) {
    rotationSpeed = speed;
}

void Spotlight::SetCutoffAngles(float inner, float outer) {
    cutoffAngle = inner;
    outerCutoffAngle = outer;
}

void Spotlight::SendToShader(GLuint programId) {
    GLuint spotPosLoc = glGetUniformLocation(programId, "spotlight.position");
    GLuint spotDirLoc = glGetUniformLocation(programId, "spotlight.direction");
    GLuint spotCutoffLoc = glGetUniformLocation(programId, "spotlight.cutOff");
    GLuint spotOuterCutoffLoc = glGetUniformLocation(programId, "spotlight.outerCutOff");
    GLuint spotAmbientLoc = glGetUniformLocation(programId, "spotlight.ambient");
    GLuint spotDiffuseLoc = glGetUniformLocation(programId, "spotlight.diffuse");
    GLuint spotSpecularLoc = glGetUniformLocation(programId, "spotlight.specular");
    GLuint spotConstantLoc = glGetUniformLocation(programId, "spotlight.constant");
    GLuint spotLinearLoc = glGetUniformLocation(programId, "spotlight.linear");
    GLuint spotQuadraticLoc = glGetUniformLocation(programId, "spotlight.quadratic");

    glUniform3fv(spotPosLoc, 1, glm::value_ptr(position));
    glUniform3fv(spotDirLoc, 1, glm::value_ptr(direction));
    glUniform1f(spotCutoffLoc, glm::cos(glm::radians(cutoffAngle)));
    glUniform1f(spotOuterCutoffLoc, glm::cos(glm::radians(outerCutoffAngle)));
    glUniform3fv(spotAmbientLoc, 1, glm::value_ptr(ambientColor));
    glUniform3fv(spotDiffuseLoc, 1, glm::value_ptr(diffuseColor));
    glUniform3fv(spotSpecularLoc, 1, glm::value_ptr(specularColor));
    glUniform1f(spotConstantLoc, constant);
    glUniform1f(spotLinearLoc, linear);
    glUniform1f(spotQuadraticLoc, quadratic);
}

//Nou
void Spotlight::RenderLightSource(GLuint vaoId, GLuint matLoc, glm::mat4 baseMatrix) {
    if (!renderLightSource) return;

    // Matrice pentru sfera luminoasa
    glm::mat4 lightMatrix = glm::translate(glm::mat4(1.0f), position);
    lightMatrix = glm::scale(lightMatrix, glm::vec3(lightSourceRadius));
    lightMatrix = baseMatrix * lightMatrix;

    glUniformMatrix4fv(matLoc, 1, GL_FALSE, &lightMatrix[0][0]);
}

void Spotlight::SetLightSourceRadius(float radius) {
    lightSourceRadius = radius;
}

void Spotlight::SetRenderLightSource(bool render) {
    renderLightSource = render;
}

void Spotlight::ToggleRotation() {
    isRotating = !isRotating;
}

void Spotlight::SetRotating(bool rotating) {
    isRotating = rotating;
}