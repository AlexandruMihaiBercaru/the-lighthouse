#version 410 core

in vec4 fragColor;
in vec2 fragUV;

out vec4 out_Color;

void main() {    
    // distanta de la centrul particulei pentru fiecare fragment
     float dist = length(fragUV - vec2(0.5));

     // daca acel fragment e in afara cercului, nu il mai randam
     if (dist > 0.5) {
         discard;
     }

     float shapeAlpha = 1.0 - smoothstep(0.4, 0.5, dist);
     float lifeAlpha = fragColor.a;
     
     out_Color = vec4(fragColor.xyz, shapeAlpha * lifeAlpha);
}
