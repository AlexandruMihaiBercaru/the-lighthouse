//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laborator X - Shader-ul pentru solutie
// =====================================
// 
//  Shaderul de fragment / Fragment shader - afecteaza culoarea pixelilor;
//


#version 330 core

in vec3 FragPos;  
in vec3 Normal; 
in vec3 inViewPos;
in vec3 inLightPos;
in vec4 ex_Color;
in vec2 TexCoord;


out vec4 out_Color;

 //  Proprietatile sursei de lumina;
vec3 lightColor = vec3 (0.95, 0.95, 0.98);
vec3 objectColor = ex_Color.xyz;
uniform int codCol;
uniform int objectId;
uniform int fogEnable;
uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;
uniform sampler2D texture_diffuse1; // Assimp loads this name by default -> pentru modele

// Spotlight structure
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight spotlight;
uniform int spotlightEnable;

void main(void)
  {
    //efectul de ceata - implementarea 1
    /* float z = distance(inViewPos, FragPos);
    float density = 0.01;
    float fogFactor = exp(-density * z);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    
    vec3 fogColor = vec3(0.57f, 0.57f, 0.5f); */ 

    // efectul de ceata - implementarea de la dl prof
    vec3 fogColor = vec3(0.5, 0.5, 0.5);
    float dist=length(inViewPos - FragPos);
    float fogFactor=exp(-0.005*dist); // intre 0 si 1; 1 corespunde aproape de obiect
    //out_Color = vec4(mix(fogColor,result,fogFactor), 1.0f);
   

    
    if (codCol == 0){
    

        //  Ambient;
        float ambientStrength = 0.5f;
        vec3 ambient_light = ambientStrength * lightColor;          //  ambient_light=ambientStrength*lightColor; 
        vec3 ambient_term= ambient_light * objectColor;             //  ambient_material=objectColor;
  	
        //  Diffuse; 
        vec3 norm = normalize(Normal);                              //  vectorul s;
        vec3 lightDir = normalize(inLightPos - FragPos);            //  vectorul L;
        float diff = max(dot(norm, lightDir), 0.0); 
        vec3 diffuse_light = lightColor;                            //  diffuse_light=lightColor;
        vec3 diffuse_term = diff * diffuse_light *  objectColor;     //  diffuse_material=objectColor;
    
        //  Specular;
        float specularStrength = 0.2f;
        float shininess = 1.0f;
        vec3 viewDir = normalize(inViewPos - FragPos);              //  versorul catre observator;
        vec3 reflectDir = normalize(reflect(-lightDir, norm));      //  versorul vectorului R;
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); 
        vec3 specular_light = specularStrength  * lightColor;       //  specular_light=specularStrength  * lightColor;
        vec3 specular_term = spec * specular_light * objectColor;   //  specular_material=objectColor;
       
        //  Culoarea finala; 
        vec3 emission=vec3(0.0, 0.0, 0.0);
        vec3 result = emission + (ambient_term + diffuse_term + specular_term);

        // ============ SPOTLIGHT CALCULATION ============
        if (spotlightEnable == 1) {
            vec3 spotLightDir = normalize(spotlight.position - FragPos);
        
        // Verificam daca fragmentul este in conul de lumina
        float theta = dot(spotLightDir, normalize(-spotlight.direction));
        float epsilon = spotlight.cutOff - spotlight.outerCutOff;
        float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);
        
	    // Calculam atenuarea distantei
        float distance = length(spotlight.position - FragPos);
        float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance + spotlight.quadratic * (distance * distance));
        
        // Componentele spotlight
        vec3 spot_ambient = spotlight.ambient * objectColor;
        
        float spot_diff = max(dot(norm, spotLightDir), 0.0);
        vec3 spot_diffuse = spotlight.diffuse * spot_diff * objectColor;
        
        vec3 spot_reflectDir = reflect(-spotLightDir, norm);
        float spot_spec = pow(max(dot(viewDir, spot_reflectDir), 0.0), shininess * 32.0);
        vec3 spot_specular = spotlight.specular * spot_spec * objectColor;
        
        // Aplicam intensitatea si atenuarea
        spot_ambient *= attenuation * intensity;
        spot_diffuse *= attenuation * intensity;
        spot_specular *= attenuation * intensity;

	    // Adaugam contributia spotlight-ului
        result += (spot_ambient + spot_diffuse + spot_specular);
        }
        // ============ END SPOTLIGHT ============

        // daca vrem setarea cu ceata
        if (fogEnable == 1){         
            vec4 color_fog = vec4(mix(fogColor, result, fogFactor), 1.0);
	        out_Color = color_fog;
        }    
            else{
                out_Color = vec4(result, 1.0);
        }

 
            
    }
        

    if (codCol == 1){ // pentru umbre
        vec3 black = vec3 (0.0, 0.0, 0.0);
        if (fogEnable == 0){
            out_Color = vec4(black, 1.0);
        }
        else{
            vec4 color_fog = vec4(mix(fogColor, black, fogFactor), 1.0);
		    out_Color = color_fog;
        }
    }

     float p = 0.25f;

     if(objectId == 8){ //texturi
            out_Color = texture(ourTexture,TexCoord)  *p+ out_Color*(1-p);
     }

     if(objectId == 9){
            out_Color = texture(texture_diffuse1,TexCoord);
     }

     if(objectId == 10){ // ID pentru cottage
            out_Color = texture(texture_diffuse1, TexCoord);
     }

     if(objectId == 11){ // ID pentru boat1
            out_Color = texture(texture_diffuse1, TexCoord);
     }

     if(objectId == 12){ // ID pentru boat1
            out_Color = texture(texture_diffuse1, TexCoord);
     }

}