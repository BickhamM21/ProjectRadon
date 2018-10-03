

//
//  Mesh.cpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/11/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#include "Mesh.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "VAO.hpp"

Mesh :: Mesh(Shader Shader, VAO Vao, Camera &C)
: shader(Shader), vao(Vao), c(C)
{
    //vao.BindVAO();
    //vao.BindVAO();

    pos = glm::vec3(-8,-8,-8);
    

    
    //SetVBOData(data, GL_STATIC_DRAW);
    //SetEBOData(indices, GL_STATIC_DRAW);
    
    
    //vao.EnableAttrib(0);

    //vao.editVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

}


void Mesh :: SetVBOData(glm::vec3 data[], int drawMode, int Size) {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Size, data, drawMode);

}

void Mesh :: SetEBOData(int data[], int drawMode, int Size) {
    EBOSize = Size/sizeof(int);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size, data, drawMode);
}

void Mesh :: SetNormalVBOData(glm::vec3 data[], int drawMode, int Size) {
    glGenBuffers(1, &NormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
    glBufferData(GL_ARRAY_BUFFER, Size, data, drawMode);
}

void Mesh :: BindBuffers() {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glGenBuffers(1, &NormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void Mesh :: InitVAO() {
    
    
    
    
    
    
}

void Mesh :: Update() {
    
    glUseProgram(shader.programID);
    shader.UpdateMatrices(c, *this);
    
    vao.EnableAttrib(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    vao.editVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);
    

    
    vao.EnableAttrib(2);
    glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
    vao.editVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);
    
    
    glDrawElements(GL_TRIANGLES, EBOSize, GL_UNSIGNED_INT, 0);


    //glDrawArrays(GL_TRIANGLES, 0, EBOSize);
    vao.UnBindVAO();
    vao.BindVAO();
}
