//
//  Camera.cpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/13/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#include "Camera.hpp"
#include <glm/glm.hpp>
#include <iostream>

Camera:: Camera() {
    firstMouse = true;
    pos = glm::vec3(0,0,0);
    speed = .1f;
    
    lastX = 640;
    lastY = 400;
    
}

Camera:: Camera(glm::vec3 Pos)
: pos(Pos)
{
    firstMouse = true;
    speed = .1f;
    
    //glfwSetCursorPosCallback(window.GetWindow(), &Camera::mouseCallBack);

    
}

void Camera::UpdateCamera(GLFWwindow *window) {
    
    float actual;
    
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        actual = speed * 10;
    } else {
        actual = speed;
    }
    
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        MoveCameraZ(actual);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        MoveCameraX(actual);

    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        MoveCameraZ(-actual);

    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        MoveCameraX(-actual);

    }
    
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        MoveCameraY(-actual);
    }
    
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        MoveCameraY(actual);
    }
    
    
    glfwGetCursorPos(window, &lastX, &lastY);
    //std::cout << lastX << std::endl;
    
}

void Camera::MoveCameraX(float x) {
    pos.x += x;
}

void Camera::MoveCameraY(float y) {
    pos.y += y;

}

void Camera::MoveCameraZ(float z) {
    pos.z += z;

}

void Camera::mouseCallBack(GLFWwindow *window, double xpos, double ypos) {
    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        
        firstMouse = false;
    }
    
    float xOffset = xpos - lastX;
    float yOffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.5f;
    
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    
    
    
    
}

