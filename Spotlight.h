#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <GL/glew.h>

class Spotlight {
private:
    glm::vec3 position;           // Pozitia spotlight-ului (in varful farului)
    glm::quat orientation;        // Orientarea folosind cuaternioni
    glm::vec3 direction;          // Directia curenta de iluminare
    glm::vec3 baseDirection;      // Directia initiala (inainte de rotatie)

    float rotationSpeed;          // Viteza de rotatie (radiani/secunda)
    float cutoffAngle;            // Unghiul de deschidere al conului (grade)
    float outerCutoffAngle;       // Unghi exterior pentru atenuare (grade)

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    float constant;               // Factori de atenuare
    float linear;
    float quadratic;

    //Nou
    bool renderLightSource;
    float lightSourceRadius;
    bool isRotating;

public:
    Spotlight(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 100.0f),
        float cutoff = 25.0f,
        float outerCutoff = 35.0f,
        float speed = 0.5f);

    void Update(float deltaTime);
    void SetPosition(glm::vec3 pos);
    void SetRotationSpeed(float speed);
    void SetCutoffAngles(float inner, float outer);
    //Nou
    void RenderLightSource(GLuint vaoId, GLuint matLoc, glm::mat4 baseMatrix);
    void SetLightSourceRadius(float radius);
    void SetRenderLightSource(bool render);
    void ToggleRotation();
    void SetRotating(bool rotating);
    bool IsRotating() const { return isRotating; }
    bool ShouldRenderLightSource() const { return renderLightSource; }

    // Getteri pentru shadere
    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetDirection() const { return direction; }
    float GetCutoff() const { return cutoffAngle; }
    float GetOuterCutoff() const { return outerCutoffAngle; }
    glm::vec3 GetAmbient() const { return ambientColor; }
    glm::vec3 GetDiffuse() const { return diffuseColor; }
    glm::vec3 GetSpecular() const { return specularColor; }
    float GetConstant() const { return constant; }
    float GetLinear() const { return linear; }
    float GetQuadratic() const { return quadratic; }

    // Transmite parametrii catre shader
    void SendToShader(GLuint programId);
};