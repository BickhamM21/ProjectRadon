//
//  Shader.hpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/11/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Camera.hpp"


class Mesh;

class Shader {
public:
    Shader(const char * vertex_file_path,const char * fragment_file_path);
    
    void setTransformMatrix(glm::mat4 transform);
    void setViewMatrix(glm::mat4 view);
    void setProjectionMatrix(glm::mat4 projection);

    void setViewPos(glm::vec3 ViewPos) {
        
    }
    
    void UpdateMatrices(Camera c, Mesh m);
    int programID;
    glm::mat4 transformMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    
    glm::vec3 viewPos;
    
    int transformLocation;
    int viewLocation;
    int projectionLocation;
    int viewPosLocation;
private:
    
    
};


#endif /* Shader_hpp */
