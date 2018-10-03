//
//  Window.cpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/10/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#include <GL/glew.h>
#include <iostream>
#include "Window.hpp"


#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

Window :: Window (int Width, int Height, char* Title)
: width(Width), height(Height), title(Title)
{
    
    if(!glfwInit()) {
        std::cout << "YOUR GLFW DONE OOFED";
    }
    
    glfwSetErrorCallback(error_callback);

    
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 2);
    
    
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!window) {
        std::cout << "YOUR WINDOW DONE OOFED";

    }
    
    glfwMakeContextCurrent(window);
    glewInit();
    glClearColor(0, 0, 0, 1);
   
    
    
}




