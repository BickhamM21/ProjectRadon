//
//  main.cpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/10/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Window.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include <glm/glm.hpp>
#include "VAO.hpp"
#include "MarchingCubes.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include <OpenCL/cl.h>
#include <boost/thread.hpp>
#include "CLKernel.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    
}



void mouseCallBack(GLFWwindow *window, double xpos, double ypos) {
    
}

int main(int argc, const char * argv[]) {
    // insert code here...

    Window window(2560,1600,"Project Radon");
    //Mesh mesh;
    Shader shader("vertShader.glsl", "fragmentShader.glsl");

    //Start OpenCl test code


    //Create the camera object
    Camera c;


    MarchingCubes *m = new MarchingCubes(32, 32, 32, 1.0f, 0.0f, 8, glm::vec3(-0, -0, -0));


    VAO vao;
    vao.BindVAO();

    //make sure to use the reference of the camera or the positions will not update correctly, if you dont use a reference, then the copied
    //camera going into the mesh class will not update positions like is should
    Mesh mesh(shader, vao, std::ref(c));

    m->UpdateMesh();
    mesh.SetVBOData(&m->vertices[0], GL_DYNAMIC_DRAW, m->vertices.size() * sizeof(glm::vec3));
    mesh.SetNormalVBOData(&m->normals[0], GL_DYNAMIC_DRAW, m->normals.size() * sizeof(glm::vec3));
    mesh.SetEBOData(&m->indices[0], GL_DYNAMIC_DRAW, m->indices.size() * sizeof(int));

    //mesh.InitVAO();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glfwSwapInterval(1);

    //0 = vsync off
    //1 = vysnc on

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);



    glfwSetCursorPosCallback(window.GetWindow(), mouseCallBack);

    //glfwSetCursorPos(window.GetWindow(), 640, 400);
    bool firstLoop = true;
    //main game loop


    CLKernel CLK("SimplexNoise.cl");

    while(!glfwWindowShouldClose(window.GetWindow())) {
        float firsttime = glfwGetTime();

        glfwSwapBuffers(window.GetWindow()); //swap the buffer in the GPU memory, this is needed

        if(firstLoop) {
            glfwSetCursorPos(window.GetWindow(), 640, 400);

            firstLoop = false;
        }

        c.UpdateCamera(window.GetWindow()); //update Camera positions based on inoute

        glfwPollEvents(); //poll for input events


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the screen for a new drawing
        //m->pos = glm::vec3(glfwGetTime(),0,0);
        //m->UpdateMesh(); //update things like the shader attached to the mesh, also, draw the mesh




        /*mesh.SetVBOData(&m->vertices[0], GL_DYNAMIC_DRAW,
                        m->vertices.size() * sizeof(glm::vec3));
        mesh.SetNormalVBOData(&m->normals[0], GL_DYNAMIC_DRAW,
                              m->normals.size() * sizeof(glm::vec3));
        mesh.SetEBOData(&m->indices[0], GL_DYNAMIC_DRAW, m->indices.size() * sizeof(int)); *///set the indices data


        if(m->vertices.size() != 0) {
            mesh.Update(); //ONLY UPDATE THE MESH IF THE SIZE OF THE VERTICES BUFFER IS NOT 0, IF IT IS ZERO, ERRORS WILL OCCUR
        }


        glDisableVertexAttribArray(0); //unbind the VAO buffers (vertices)
        glDisableVertexAttribArray(2); //unbind the VAO buffers (normals)


        float secondtime = glfwGetTime();

        std::cout << 1000 * (secondtime - firsttime) << std::endl;
    }

    glfwDestroyWindow(window.GetWindow()); //Destroy the Window since the application was closed
    glfwTerminate(); //terminate the instace of GLFW


    return 0;
}
