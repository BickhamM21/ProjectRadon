//
//  Window.hpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/10/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <GL/glew.h>

#include <stdio.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int Width, int Height, char* Title);

    GLFWwindow* GetWindow() {return window;}
private:
    int width;
    int height;
    char* title;
    GLFWwindow* window;
};

#endif /* Window_hpp */
