//
//  Camera.hpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/13/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


class Camera {
    
public:
    
    Camera();
    Camera(glm::vec3 Pos);
    
    glm::mat4 getViewMatrix() { return viewMatrix;}
    glm::mat4 getProjectionMatrix() {return projectionMatrix;}
    
    void MoveCameraX(float x);
    void MoveCameraY(float y);
    void MoveCameraZ(float z);
    void UpdateCamera(GLFWwindow *window);
    
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 pos;
    float speed;
    float pitch;
    float yaw;
    float roll;
    bool firstMouse;
    
    double lastX;
    double lastY;
    
    
    
    void mouseCallBack(GLFWwindow *window, double xpos, double ypos);
private:
   
};


#endif /* Camera_hpp */

