#version 410 core

in vec4 vColor;
in vec3 vNormal;
in vec3 FragPos;

out vec4 FragColor;

uniform float time;
uniform vec3 ViewPos;

uniform vec3 lightPos;


void main()
{
    vec3 norm = normalize(vNormal);
    
    //using light position as first value to be substracted from
    vec3 lightDir = normalize(vec3(0,-10,-10) - FragPos);
    
    
    //using view position to calculate the specular light
    float specularStrength = 1.0f;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    float spec = pow(max(dot(norm, halfwayDir), 0), 128);
    vec3 specular = spec * vec3(1.0f, 1.0f,1.0f) * specularStrength;
    
    vec3 diff = vec3(max(dot(norm,lightDir),0.0));
    
    float attenuationFactor = 0.0001f;
    float distanceFromLight = distance(vec3(0,-10,-10), FragPos);
    float attenuation = 1/(1.0 + attenuationFactor * pow(distanceFromLight,2));
    
    vec3 totalLight = attenuation * (diff + specular);
    
    FragColor = vec4(totalLight, 1) * vec4(1.0f,1.0f,1.0f,1);//vec4(noise3(vec3(vColor.x, vColor.y, vColor.z) + time),1);
}
