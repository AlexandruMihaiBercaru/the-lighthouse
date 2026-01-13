#version 410 core

layout(location = 0) in vec3 in_Position;  // Atributul 0 = coordonatele vf
layout(location = 1) in vec3 in_Color;     // Atributul 1 = culoarea vf

out vec3 vPosition;
out vec3 vColor;

void main() {
    vPosition = in_Position;
    vColor = in_Color;
}