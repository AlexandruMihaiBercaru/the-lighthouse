#version 410 core

layout(lines) in; // Tess Eval Shader cu layout = isolines trimite mai departe segmente de dreapta
layout(triangle_strip, max_vertices = 4) out; // vrem sa trimitem mai departe catre Fragment Shader mici patrate 
// (folosim primitiva triangle strip)


in vec4 teColor[]; // de la TES

out vec4 fragColor; // trimitem culoarea catre Fragment
out vec2 fragUV; // coordonatele (u,v)

uniform mat4 projection;
uniform float particleSize = 2.0;
uniform mat4 view;

void main() {

   // Transform segmentul de dreapta in puncte
    vec3 pos = gl_in[0].gl_Position.xyz;
    vec4 color = teColor[0];

    // Nu trimit mai departe acele varfuri care au fost eliminate in TES
    if (gl_in[0].gl_Position.w == 0.0) return;

    // construim cate un nou varf care sa corespunda fiecarui colt al patratului
    float jum = particleSize / 2.0;

    // dreapta-sus
    gl_Position = projection * vec4(pos.x + jum, pos.y + jum, pos.z, 1.0);
    fragUV = vec2(1.0, 1.0);
    fragColor = color;
    EmitVertex();

    // dreapta-jos
    gl_Position = projection * vec4(pos.x + jum, pos.y - jum, pos.z, 1.0);
    fragUV = vec2(1.0, 0.0);
    fragColor = color;
    EmitVertex();

    // stanga-sus
    gl_Position = projection * vec4(pos.x - jum, pos.y + jum, pos.z, 1.0);
    fragUV = vec2(0.0, 1.0);
    fragColor = color;
    EmitVertex();

    // stanga-jos
    gl_Position = projection * vec4(pos.x - jum, pos.y - jum, pos.z, 1.0);
    fragUV = vec2(0.0, 0.0);
    fragColor = color;
    EmitVertex();

    EndPrimitive();
}