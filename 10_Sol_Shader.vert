//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul X - Shader-ul pentru solutie
// =====================================
// 
//  Shaderul de varfuri / Vertex shader - afecteaza geometria scenei; 
 
 #version 330 core


layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 inTexture;
layout(location=3) in vec3 in_Color;


//  Variabile de iesire;
out vec3 FragPos;
out vec3 Normal;
out vec3 inViewPos;
out vec3 inLightPos;
out vec4 ex_Color;
out vec2 TexCoord;

//  Variabile uniforme;
uniform int nrVertices;
uniform int objectId;
uniform int codCol;
uniform mat4 myMatrix;
uniform vec3 viewPos;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 matrUmbra;
uniform mat4 matrUmbraApa;
uniform vec3 lightPos;

uniform sampler2D myTexture;

void main(void)
  {

    

    // Transformari in spatiul de modelare
    if (codCol == 0)
        gl_Position = projection * view * myMatrix * vec4(in_Position, 1.0);
    else if (codCol == 1){
        gl_Position = projection * view * matrUmbra * myMatrix *  vec4(in_Position, 1.0);
    }else if(codCol == -1){
        gl_Position = projection * view * matrUmbraApa * myMatrix *  vec4(in_Position, 1.0);
    }

    // Pentru modelul de iluminare 
    // Atribute ale varfurilor
    FragPos = mat3(myMatrix) * in_Position;
    Normal = mat3(myMatrix) * in_Normal;


  
    // Pozitia sursei de lumina (coincide cu a observatorului)

    // Pozitia observatorului
    //inViewPos = vec3(myMatrix * vec4(viewPos, 1.0f));;
    inLightPos = vec3(myMatrix * vec4(lightPos, 1.0f));
    // Sursa de lumina
    //inLightPos = vec3(myMatrix * vec4(lightPos, 1.0f));
    inViewPos = vec3(myMatrix * vec4(viewPos, 1.0f));

    
    // Pozitia sursei de lumina (diferita de a observatorului)
    // inLightPos = vec3(5.0, 5.0, 7.0);

    // Culoarea varfurilor
 	// ex_Color=vec4(1.2 * gl_VertexID/nrVertices, 0.95 * gl_VertexID/nrVertices, 1.5 * gl_VertexID/nrVertices, 1.0);
    if (codCol == 0){
        switch(objectId){
            case 0:
                 ex_Color = vec4(in_Color, 1.0);
                 break;

            case 2:
                ex_Color = vec4(0.05, 0.05, 0.05, 1.0);
                break;
            case 3:
                ex_Color = vec4(0.1, 0.1, 0.1, 1.0);
                break;
            case 4: //white
                ex_Color = vec4(1.0, 1.0, 1.0, 1.0);
                break;
            case 5: //red
                ex_Color = vec4(1.0, 0.0, 0.0, 1.0);
                break;
            case 6: //green
                ex_Color = vec4(0.0, 1.0, 0.0, 1.0);
                break;
            case 7://wood
                ex_Color = vec4(0.55, 0.27, 0.07, 1.0);
                break;
            case 8://texture loaded;
                 TexCoord = inTexture;
                 break;
            case 9://texture for imported models;
                 TexCoord = inTexture;
                 break;
            case 10://texture for imported models;
                 TexCoord = inTexture;
                 break;
            case 11://texture for imported models;
                 TexCoord = inTexture;
                 break;
            case 12://texture for imported models;
                 TexCoord = inTexture;
                 break;
        }
    }


} 