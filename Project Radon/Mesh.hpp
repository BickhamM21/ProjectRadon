//
//  Mesh.hpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/11/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "VAO.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

class Mesh {
public:
    Mesh(Shader Shader, VAO Vao, Camera &C);
    Shader shader;
    VAO vao;
    
    void BindBuffers();
    
    GLuint getVBOID() { return VBO;}
    GLuint getEBOID() { return EBO;}
    
    void SetVBOData(glm::vec3 data[], int drawMode, int Size);
    void SetEBOData(int data[], int drawMode, int Size);
    void SetNormalVBOData(glm::vec3 data[], int drawMode, int Size);
    
    
    void InitVAO();
    void Update();
    
    int EBOSize;
    
    GLuint VBO;
    GLuint EBO;
    GLuint NormalVBO;

    
    Camera &c;
    glm::vec3 pos;
    
private:
    
 
};

#endif /* Mesh_hpp */
