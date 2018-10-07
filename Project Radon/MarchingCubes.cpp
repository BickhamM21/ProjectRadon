//
//  MarchingCubes.cpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/13/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#include "MarchingCubes.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <boost/thread.hpp>
#include <ctpl.h>


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif



MarchingCubes::MarchingCubes(int chunkSizeX, int chunkSizeY, int chunkSizeZ, float gridSize, float isoValue,
                             int numThreads, glm::vec3 pos, CLKernel Kernel)
        : ChunkSizeX(chunkSizeX), ChunkSizeY(chunkSizeY), ChunkSizeZ(chunkSizeZ), GridSize(gridSize),
          IsoValue(isoValue), pos(pos), kernel(Kernel) {
    float onetime = glfwGetTime();

    int error;


    float settings[7];
    settings[0] = chunkSizeX;
    settings[1] = chunkSizeY;
    settings[2] = chunkSizeZ;
    settings[3] = gridSize;
    settings[4] = pos.x;
    settings[5] = pos.y;
    settings[6] = pos.z;



    const int SIZE = (ChunkSizeX + 1) * (ChunkSizeY + 1) * (ChunkSizeZ + 1);

    cl_command_queue queue = clCreateCommandQueue(kernel.context, kernel.device_id, 0, &error);


    cl_mem voxels_mem_obj = clCreateBuffer(kernel.context, CL_MEM_READ_WRITE, SIZE * sizeof(float), NULL, &error);
    cl_mem settings_mem_obj = clCreateBuffer(kernel.context, CL_MEM_READ_ONLY, 7 * sizeof(float), NULL, &error);
    cl_mem norms_mem_obj = clCreateBuffer(kernel.context, CL_MEM_READ_WRITE, SIZE * sizeof(glm::vec3), NULL, &error);


    error = clEnqueueWriteBuffer(queue, settings_mem_obj, CL_TRUE, 0, 7 * sizeof(float), settings, 0, NULL, NULL);

    error = clSetKernelArg(kernel.kernel, 0, sizeof(cl_mem), (void *) &voxels_mem_obj);
    error = clSetKernelArg(kernel.kernel, 1, sizeof(cl_mem), (void *) &settings_mem_obj);
    error = clSetKernelArg(kernel.kernel, 2, sizeof(cl_mem), (void *) &norms_mem_obj);

    size_t global_item_sizes[3];
    global_item_sizes[0] = ChunkSizeX + 1;
    global_item_sizes[1] = ChunkSizeY + 1;
    global_item_sizes[2] = ChunkSizeZ + 1;






    //size_t local_item_size = 33;


    error = clEnqueueNDRangeKernel(queue, kernel.kernel, 3, nullptr, global_item_sizes, nullptr, 0, nullptr, nullptr);

    voxels = (float *) malloc(sizeof(float) * SIZE);

    error = clEnqueueReadBuffer(queue, voxels_mem_obj, CL_TRUE, 0, SIZE * sizeof(float), voxels, 0, nullptr, nullptr);

    //std::cout << error << std::endl;

    float twotime = glfwGetTime();


    std::cout<< twotime - onetime << std::endl;


    //voxels = C;

    VerticesTime = 0;
    
    std::vector<glm::vec3> vertices0;
    std::vector<int> indices0;
    std::vector<glm::vec3> normals0;
    
    std::vector<glm::vec3> vertices1;
    std::vector<int> indices1;
    std::vector<glm::vec3> normals1;

    std::vector<glm::vec3> vertices2;
    std::vector<int> indices2;
    std::vector<glm::vec3> normals2;

    std::vector<glm::vec3> vertices3;
    std::vector<int> indices3;
    std::vector<glm::vec3> normals3;

    std::vector<glm::vec3> vertices4;
    std::vector<int> indices4;
    std::vector<glm::vec3> normals4;

    std::vector<glm::vec3> vertices5;
    std::vector<int> indices5;
    std::vector<glm::vec3> normals5;

    std::vector<glm::vec3> vertices6;
    std::vector<int> indices6;
    std::vector<glm::vec3> normals6;

    std::vector<glm::vec3> vertices7;
    std::vector<int> indices7;
    std::vector<glm::vec3> normals7;

    
    
    MCData data0(chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 8, 0, std::ref(vertices0), std::ref(indices0), std::ref(normals0));
    MCData data1(chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 8, 1, std::ref(vertices1), std::ref(indices1), std::ref(normals1));
    MCData data2(chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 8, 2, std::ref(vertices2), std::ref(indices2), std::ref(normals2));
    MCData data3(chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 8, 3, std::ref(vertices3), std::ref(indices3), std::ref(normals3));

    
    MCData data4(chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 8, 4, std::ref(vertices4), std::ref(indices4), std::ref(normals4));
    MCData data5(chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 8, 5, std::ref(vertices5), std::ref(indices5), std::ref(normals5));
    MCData data6(chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 8, 6, std::ref(vertices6), std::ref(indices6), std::ref(normals6));
    MCData data7(chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 8, 7, std::ref(vertices7), std::ref(indices7), std::ref(normals7));


    //p.push(&MarchingCubes::processSection, chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 8, 0, std::ref(vertices0), std::ref(indices0));
    //p.push(&MarchingCubes::processSection, (chunkSizeX, chunkSizeY, chunkSizeZ, gridSize, isoValue, pos, 9, 0, std::ref(vertices0), std::ref(indices0)));


    ctpl::thread_pool threadPool(8);

    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data0));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data1));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data2));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data3));

    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data4));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data5));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data6));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data7));

    //p.push();

    //std::thread t1(&MarchingCubes::processSection, std::ref(*this),0, data0);
    //std::thread t2(&MarchingCubes::processSection, std::ref(*this),0, data1);
    //std::thread t3(&MarchingCubes::processSection, std::ref(*this),0, data2);
    //std::thread t4(&MarchingCubes::processSection, std::ref(*this),0, data3);

    //std::thread t5(&MarchingCubes::processSection, std::ref(*this),0, data4);
    //std::thread t6(&MarchingCubes::processSection, std::ref(*this),0, data5);
    //std::thread t7(&MarchingCubes::processSection, std::ref(*this),0, data6);
    //std::thread t8(&MarchingCubes::processSection, std::ref(*this),0, data7);
    
    


    threadPool.stop(true);
    //t1.join();

    for(int i = 0; i < vertices0.size(); i++) {
        vertices.push_back(vertices0[i]);
    }
    
    //t2.join();

    
    for(int i = 0; i < vertices1.size(); i++) {
        vertices.push_back(vertices1[i]);
    }
    
    //t3.join();

    
    for(int i = 0; i < vertices2.size(); i++) {
        vertices.push_back(vertices2[i]);
    }
    
    //t4.join();
    
    for(int i = 0; i < vertices3.size(); i++) {
        vertices.push_back(vertices3[i]);
    }
    
    //t5.join();
    
    for(int i = 0; i < vertices4.size(); i++) {
        vertices.push_back(vertices4[i]);
    }
    
    //t6.join();
    
    for(int i = 0; i < vertices5.size(); i++) {
        vertices.push_back(vertices5[i]);
    }
    
    //t7.join();
    
    for(int i = 0; i < vertices6.size(); i++) {
        vertices.push_back(vertices6[i]);
    }
    
    //t8.join();
    double firstTime = glfwGetTime();

    
    for(int i = 0; i < vertices7.size(); i++) {
        vertices.push_back(vertices7[i]);
    }
    
    //t8.detach();

    
    double secondTime = glfwGetTime();
    
    //std::reverse(indices.begin(), indices.end());
    //std::cout << secondTime - firstTime << std::endl;

    
    for(int i = 0; i < vertices.size(); i ++) {
        indices.push_back(i);
    }
    
    //vertices = vertices0;
    //indices = indices0;
}

glm::vec3 MarchingCubes :: VertexInterp(float IsoValue, glm::vec3 p1, glm::vec3 p2, float v1, float v2) {
    
    
    float firsttime = glfwGetTime();
    
    double mu;
    glm::vec3 p;
    
    if(abs(IsoValue - v1) < 0.00001) {
        return(p1);
    }
    
    if(abs(IsoValue - v2) < 0.00001) {
        return(p2);
    }
    
    if(abs(v1 - v2) < 0.00001) {
        return(p1);
    }
    
    mu = (IsoValue - v1)/(v2 - v1);
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    //p.x = (p1.x + p2.x)/2;
    //p.y = (p1.y + p2.y)/2;
    //p.z = (p1.z + p2.z)/2;
    float secondTime = glfwGetTime();
    VerticesTime += secondTime - firsttime;
    
    return(p);
}

float MarchingCubes::Circle(glm::vec3 pos) {

    float radius = 125;

    float z = pos.z - 8; //+ cos(glfwGetTime()) * 5;
    float y = pos.y + radius - 10;
    float x = pos.x - 8; //+ sin(glfwGetTime()) * 5;
    
    
    float circle = (x * x + y * y + z * z - (radius * radius));

    
    
    return circle/(10000);//(z * z - x * x + y);//((y * y) + (x * x) + (z * z)/4 - 9) + sin(x) * sin(y) * sin(z) * 5;
}

glm::vec3 MarchingCubes::NormalCalc(glm::vec3 pos) {
    const float H = .001;

    int x1 = floor(pos.x);
    int y1 = floor(pos.y);
    int z1 = floor(pos.z);

    const float v = voxels[x1 + y1 * (ChunkSizeX + 1) + z1 * (ChunkSizeX + 1) * (ChunkSizeY + 1)];
    const float dxn = v - voxels[(x1 + 1) + y1 * (ChunkSizeX + 1) + z1 * (ChunkSizeX + 1) * (ChunkSizeY + 1)];
    const float dyn = v - voxels[(x1) + (y1 + 1) * (ChunkSizeX + 1) + z1 * (ChunkSizeX + 1) * (ChunkSizeY + 1)];
    const float dzn = v - voxels[(x1) + (y1) * (ChunkSizeX + 1) + (z1 + 1) * (ChunkSizeX + 1) * (ChunkSizeY + 1)];

    //voxels[(z1 * (ChunkSizeX + 1) * (ChunkSizeY + 1)) + (y1 * (ChunkSizeX + 1)) + x1];

    const float dx = (Circle(pos + glm::vec3(H,0,0))) - Circle(pos - glm::vec3(H,0,0));
    const float dy = Circle(pos + glm::vec3(0,H,0)) - Circle(pos - glm::vec3(0,H,0));
    const float dz = Circle(pos + glm::vec3(0,0,H)) - Circle(pos - glm::vec3(0,0,H));
    return glm::normalize(glm::vec3(dx + 0,dy + 0,dz + 0));
}

float MarchingCubes::smin(float a, float b, float k) {
    float h = glm::clamp(0.5 + 0.5*(a-b)/k, 0.0, 1.0);
    return glm::mix(a, b, h) - k*h*(1.0-h);
}

void MarchingCubes::processSection(int id, MCData data) {
    
    
    int edgeTable[256]=
    {
        0x000, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c, 0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
        0x190, 0x099, 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c, 0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
        0x230, 0x339, 0x033, 0x13a, 0x636, 0x73f, 0x435, 0x53c, 0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
        0x3a0, 0x2a9, 0x1a3, 0x0aa, 0x7a6, 0x6af, 0x5a5, 0x4ac, 0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
        0x460, 0x569, 0x663, 0x76a, 0x066, 0x16f, 0x265, 0x36c, 0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
        0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0x0ff, 0x3f5, 0x2fc, 0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
        0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x055, 0x15c, 0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
        0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0x0cc, 0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
        0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc, 0x0cc, 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
        0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c, 0x15c, 0x055, 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
        0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc, 0x2fc, 0x3f5, 0x0ff, 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
        0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c, 0x36c, 0x265, 0x16f, 0x066, 0x76a, 0x663, 0x569, 0x460,
        0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac, 0x4ac, 0x5a5, 0x6af, 0x7a6, 0x0aa, 0x1a3, 0x2a9, 0x3a0,
        0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c, 0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x033, 0x339, 0x230,
        0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c, 0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x099, 0x190,
        0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c, 0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x000
    };
    
    
    int triTable[256][16] =
    {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
        {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
        {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
        {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
        {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
        {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
        {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
        {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
        {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
        {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
        {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
        {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
        {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
        {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
        {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
        {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
        {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
        {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
        {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
        {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
        {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
        {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
        {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
        {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
        {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
        {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
        {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
        {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
        {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
        {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
        {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
        {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
        {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
        {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
        {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
        {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
        {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
        {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
        {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
        {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
        {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
        {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
        {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
        {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
        {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
        {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
        {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
        {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
        {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
        {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
        {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
        {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
        {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
        {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
        {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
        {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
        {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
        {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
        {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
        {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
        {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
        {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
        {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
        {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
        {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
        {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
        {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
        {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
        {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
        {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
        {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
        {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
        {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
        {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
        {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
        {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
        {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
        {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
        {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
        {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
        {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
        {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
        {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
        {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
        {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
        {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
        {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
        {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
        {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
        {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
        {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
        {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
        {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
        {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
        {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
        {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
        {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
        {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
        {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
        {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
        {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
        {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
        {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
        {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
        {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
        {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
        {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
        {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
        {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
        {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
        {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
        {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
        {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
        {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
        {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
        {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
        {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
        {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
        {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
        {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
        {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
        {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
        {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
        {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
        {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
        {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
        {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
        {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
        {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
        {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
        {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
        {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
        {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
        {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
        {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
        {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
        {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
        {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
        {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
        {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
        {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
        {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
        {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
        {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
        {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
        {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
        {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
        {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
        {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
        {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
        {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
        {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
        {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
        {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
        {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
        {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
        {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
        {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
        {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
        {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
        {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
        {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
        {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
        {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
        {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
        {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
        {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
        {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
        {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
        {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
        {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
        {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
        {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
        {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
        {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};
    
    
    
    
    int chunkSizeX = data.ChunkSizeX;
    int chunkSizeY = data.ChunkSizeY;
    int chunkSizeZ = data.ChunkSizeZ;
    float gridSize = data.gridSize;
    glm::vec3 pos = data.pos;
    int threadID = data.threadID;
    float isoValue = data.isoValue;
    
    int maxX = chunkSizeX/cbrt(8);
    int maxY = chunkSizeY/cbrt(8);
    int maxZ = chunkSizeZ/cbrt(8);

    glm::vec3 subDivisionLocations[8];
    subDivisionLocations[0] = glm::vec3(0,0,0);
    subDivisionLocations[1] = glm::vec3(maxX,0,0);
    subDivisionLocations[2] = glm::vec3(maxX,maxY,0);
    subDivisionLocations[3] = glm::vec3(0,maxY,0);
    
    subDivisionLocations[4] = glm::vec3(0,0,maxZ);
    subDivisionLocations[5] = glm::vec3(maxX,0,maxZ);
    subDivisionLocations[6] = glm::vec3(maxX,maxY,maxZ);
    subDivisionLocations[7] = glm::vec3(0,maxY,maxZ);
    
    
    glm::vec3 realStartPos = subDivisionLocations[threadID] * gridSize;
    
    float isoTime = 0;
    
    for(int z = 0; z < maxZ; z++) {
        for(int y = 0; y < maxY; y++) {
            
            float cachedNoise[4];
            glm::vec3 cachedVerts[4];
            
            for(int x = 0; x < maxX; x++) {
                glm::vec3 corners[8];




                corners[0] = realStartPos + glm::vec3(x * gridSize,y * gridSize,z * gridSize) + pos;
                corners[1] = corners[0] + glm::vec3(gridSize,0,0);
                corners[2] = corners[0] + glm::vec3(gridSize,gridSize,0);
                corners[3] = corners[0] + glm::vec3(0,gridSize,0);

                corners[4] = corners[0] + glm::vec3(0,0,gridSize);
                corners[5] = corners[0] + glm::vec3(gridSize,0,gridSize);
                corners[6] = corners[0] + glm::vec3(gridSize,gridSize,gridSize);
                corners[7] = corners[0] + glm::vec3(0,gridSize,gridSize);
                
                
                
                float vals[8];
                
                
                




                /*vals[0] = voxels[(x + (int)realStartPos.x) + (ChunkSizeX + 1) * (y + (int)realStartPos.y) + ((z + (int)realStartPos.z) + ((ChunkSizeX + 1) * (ChunkSizeY + 1)))];
                vals[1] = voxels[(x + 1 + (int)realStartPos.x) + (ChunkSizeX + 1) * (y + (int)realStartPos.y) + ((z + (int)realStartPos.z) + ((ChunkSizeX + 1) * (ChunkSizeY + 1)))];
                vals[2] = voxels[(x + 1 + (int)realStartPos.x) + (ChunkSizeX + 1) * (y + 1 + (int)realStartPos.y) + ((z + (int)realStartPos.z) + ((ChunkSizeX + 1) * (ChunkSizeY + 1)))];
                vals[3] = voxels[(x + (int)realStartPos.x) + (ChunkSizeX + 1) * (y + 1 + (int)realStartPos.y) + ((z + (int)realStartPos.z) + ((ChunkSizeX + 1) * (ChunkSizeY + 1)))];

                vals[4] = voxels[(x + (int)realStartPos.x) + (ChunkSizeX + 1) * (y + (int)realStartPos.y) + ((z + (int)realStartPos.z + 1) + ((ChunkSizeX + 1) * (ChunkSizeY + 1)))];
                vals[5] = voxels[(x + 1 + (int)realStartPos.x) + (ChunkSizeX + 1) * (y + (int)realStartPos.y) + ((z + (int)realStartPos.z + 1) + ((ChunkSizeX + 1) * (ChunkSizeY + 1)))];
                vals[6] = voxels[(x + 1 + (int)realStartPos.x) + (ChunkSizeX + 1) * (y + 1 + (int)realStartPos.y) + ((z + (int)realStartPos.z + 1) + ((ChunkSizeX + 1) * (ChunkSizeY + 1)))];
                vals[7] = voxels[(x + (int)realStartPos.x) + (ChunkSizeX + 1) * (y + 1 + (int)realStartPos.y) + ((z + (int)realStartPos.z + 1) + ((ChunkSizeX + 1) * (ChunkSizeY + 1)))];
*/
                /*vals[0] = voxels[(x + (int)realStartPos.x) + (ChunkSizeX + 1) * ((y+(int)realStartPos.y) + (ChunkSizeZ + 1) * (z + (int)realStartPos.z))];
                vals[1] = voxels[(x + (int)realStartPos.x  +1) + (ChunkSizeX + 1) * ((y+(int)realStartPos.y) + (ChunkSizeZ + 1) * (z + (int)realStartPos.z))];
                vals[2] = voxels[(x + (int)realStartPos.x + 1) + (ChunkSizeX + 1) * ((y+(int)realStartPos.y + 1) + (ChunkSizeZ + 1) * (z + (int)realStartPos.z))];
                vals[3] = voxels[(x + (int)realStartPos.x) + (ChunkSizeX + 1) * ((y+(int)realStartPos.y + 1) + (ChunkSizeZ + 1) * (z + (int)realStartPos.z))];


                vals[4] = voxels[(x + (int)realStartPos.x) + (ChunkSizeX + 1) * ((y+(int)realStartPos.y) + (ChunkSizeZ + 1) * (z + (int)realStartPos.z) + 1)];
                vals[5] = voxels[(x + (int)realStartPos.x  +1) + (ChunkSizeX + 1) * ((y+(int)realStartPos.y) + (ChunkSizeZ + 1) * (z + (int)realStartPos.z) + 1)];
                vals[6] = voxels[(x + (int)realStartPos.x + 1) + (ChunkSizeX + 1) * ((y+(int)realStartPos.y + 1) + (ChunkSizeZ + 1) * (z + (int)realStartPos.z) + 1)];
                vals[7] = voxels[(x + (int)realStartPos.x) + (ChunkSizeX + 1) * ((y+(int)realStartPos.y + 1) + (ChunkSizeZ + 1) * (z + (int)realStartPos.z) + 1)];*/






                int x1 = x + (realStartPos.x/gridSize);
                int y1 = y + (realStartPos.y/gridSize);
                int z1 = z + (realStartPos.z/gridSize);



                //std::cout << (int) realStartPos.y << std::endl;

                float heightMod = -y1 + 5;

                vals[0] = voxels[x1 + y1 * (ChunkSizeX + 1) + z1 * (ChunkSizeX + 1) * (ChunkSizeY + 1)];
                vals[1] = voxels[(x1 + 1) + y1 * (ChunkSizeX + 1) + z1 * (ChunkSizeX + 1) * (ChunkSizeY + 1)];
                vals[2] = voxels[(x1 + 1) + (y1 + 1) * (ChunkSizeX + 1) + z1 * (ChunkSizeX + 1) * (ChunkSizeY + 1)];
                vals[3] = voxels[x1 + (y1 + 1) * (ChunkSizeX + 1) + z1 * (ChunkSizeX + 1) * (ChunkSizeY + 1)];

                vals[4] = voxels[x1 + y1 * (ChunkSizeX + 1) + (z1 + 1) * (ChunkSizeX + 1) * (ChunkSizeY + 1)];
                vals[5] = voxels[(x1 + 1) + y1 * (ChunkSizeX + 1) + (z1 + 1) * (ChunkSizeX + 1) * (ChunkSizeY + 1)];
                vals[6] = voxels[(x1 + 1) + (y1 + 1) * (ChunkSizeX + 1) +
                                 (z1 + 1) * (ChunkSizeX + 1) * (ChunkSizeY + 1)];
                vals[7] = voxels[x1 + (y1 + 1) * (ChunkSizeX + 1) + (z1 + 1) * (ChunkSizeX + 1) * (ChunkSizeY + 1)];



                float firsttime = glfwGetTime();

                cachedNoise[0] = vals[1];
                cachedNoise[1] = vals[2];
                cachedNoise[2] = vals[6];
                cachedNoise[3] = vals[5];

                
                
                int cubeIndex = 0;
                if(vals[0] < isoValue) cubeIndex |= 1;
                if(vals[1] < isoValue) cubeIndex |= 2;
                if(vals[2] < isoValue) cubeIndex |= 4;
                if(vals[3] < isoValue) cubeIndex |= 8;
                
                if(vals[4] < isoValue) cubeIndex |= 16;
                if(vals[5] < isoValue) cubeIndex |= 32;
                if(vals[6] < isoValue) cubeIndex |= 64;
                if(vals[7] < isoValue) cubeIndex |= 128;
                
                glm::vec3 vertlist[12];
                
                
                
                
                if(edgeTable[cubeIndex != 0]) {
                    
                    if(edgeTable[cubeIndex] & 1) {
                        
                        vertlist[0] = VertexInterp(IsoValue, corners[0], corners[1], vals[0], vals[1]);
                        
                    }
                    if(edgeTable[cubeIndex] & 2) {
                        
                        vertlist[1] = VertexInterp(IsoValue, corners[1], corners[2], vals[1], vals[2]);
                        
                    }
                    if(edgeTable[cubeIndex] & 4) {
                        
                        vertlist[2] = VertexInterp(IsoValue, corners[2], corners[3], vals[2], vals[3]);
                    
                    }
                    if(edgeTable[cubeIndex] & 8) {
                        
                        if(x > 0) {
                            vertlist[3] = cachedVerts[0];

                        } else{
                            vertlist[3] = VertexInterp(IsoValue, corners[3], corners[0], vals[3], vals[0]);
                        }
                    }
                    if(edgeTable[cubeIndex] & 16) {
                        vertlist[4] = VertexInterp(IsoValue, corners[4], corners[5], vals[4], vals[5]);
                        
                    }
                    if(edgeTable[cubeIndex] & 32) {
                        vertlist[5] = VertexInterp(IsoValue, corners[5], corners[6], vals[5], vals[6]);
                        
                    }
                    if(edgeTable[cubeIndex] & 64) {
                        vertlist[6] = VertexInterp(IsoValue, corners[6], corners[7], vals[6], vals[7]);
                        
                    }
                    if(edgeTable[cubeIndex] & 128) {
                        if(x > 0) {
                            vertlist[7] = cachedVerts[2];
                        } else {
                            vertlist[7] = VertexInterp(IsoValue, corners[7], corners[4], vals[7], vals[4]);
                        }
                    }
                    if(edgeTable[cubeIndex] & 256) {
                        
                        
                        if(x > 0) {
                            vertlist[8] = cachedVerts[3];
                        } else {
                            vertlist[8] = VertexInterp(IsoValue, corners[0], corners[4], vals[0], vals[4]);
                        }
                    }
                    if(edgeTable[cubeIndex] & 512) {
                        vertlist[9] = VertexInterp(IsoValue, corners[1], corners[5], vals[1], vals[5]);
                        
                    }
                    if(edgeTable[cubeIndex] & 1024) {
                        vertlist[10] = VertexInterp(IsoValue, corners[2], corners[6], vals[2], vals[6]);
                        
                    }
                    if(edgeTable[cubeIndex] & 2048) {
                        
                        if(x > 0) {
                            vertlist[11] = cachedVerts[1];
                        } else {
                            vertlist[11] = VertexInterp(IsoValue, corners[3], corners[7], vals[3], vals[7]);
                        }
                    }
                }
                
                
                cachedVerts[0] = vertlist[1];
                cachedVerts[1] = vertlist[10];
                cachedVerts[2] = vertlist[5];
                cachedVerts[3] = vertlist[9];

                
                int idx = (x + realStartPos.x) + (y + realStartPos.y) * ChunkSizeX + (z + realStartPos.z) * ChunkSizeX * ChunkSizeY;
                
                for(int i = 0; triTable[cubeIndex][i]!= -1; i+=3) {
                    
                    data.Vertices.push_back(vertlist[triTable[cubeIndex][i + 2]]);
                    data.Vertices.push_back(vertlist[triTable[cubeIndex][i + 1]]);
                    data.Vertices.push_back(vertlist[triTable[cubeIndex][i + 0]]);
                    
                    
                    data.Normals.push_back(-NormalCalc(vertlist[triTable[cubeIndex][i + 2]]));
                    data.Normals.push_back(-NormalCalc(vertlist[triTable[cubeIndex][i + 1]]));
                    data.Normals.push_back(-NormalCalc(vertlist[triTable[cubeIndex][i + 0]]));

                    
                    //Vertices.push_back(vertlist[triTable[cubeIndex][i + 2]]);
                    
                    //std::cout << vertlist[triTable[cubeIndex][i]].x << std::endl;
                    
                    //int Index = data.Indices.size();
                    //data.Indices.push_back(Index);
                    //data.Indices.push_back(Index + 1);
                    //data.Indices.push_back(Index + 2);
                    //std::cout << Index + 2 << std::endl;
                    
                }
                
                /*int i;
                for(i = 0; i < 5; i++) {
                    
                }*/
                
                
                
            }
        }
    }
    
    //std::cout << VerticesTime << std::endl;
    
}

void MarchingCubes::UpdateMesh() {
    
    vertices.clear();
    indices.clear();
    normals.clear();
    
    VerticesTime = 0;
    
    std::vector<glm::vec3> vertices0;
    std::vector<int> indices0;
    std::vector<glm::vec3> normals0;
    
    std::vector<glm::vec3> vertices1;
    std::vector<int> indices1;
    std::vector<glm::vec3> normals1;
    
    std::vector<glm::vec3> vertices2;
    std::vector<int> indices2;
    std::vector<glm::vec3> normals2;
    
    std::vector<glm::vec3> vertices3;
    std::vector<int> indices3;
    std::vector<glm::vec3> normals3;
    
    std::vector<glm::vec3> vertices4;
    std::vector<int> indices4;
    std::vector<glm::vec3> normals4;
    
    std::vector<glm::vec3> vertices5;
    std::vector<int> indices5;
    std::vector<glm::vec3> normals5;
    
    std::vector<glm::vec3> vertices6;
    std::vector<int> indices6;
    std::vector<glm::vec3> normals6;
    
    std::vector<glm::vec3> vertices7;
    std::vector<int> indices7;
    std::vector<glm::vec3> normals7;
    
    
    
    
    MCData data0(ChunkSizeX, ChunkSizeY, ChunkSizeZ, GridSize, IsoValue, pos, 8, 0, std::ref(vertices0), std::ref(indices0), std::ref(normals0));
    MCData data1(ChunkSizeX, ChunkSizeY, ChunkSizeZ, GridSize, IsoValue, pos, 8, 1, std::ref(vertices1), std::ref(indices1), std::ref(normals1));
    MCData data2(ChunkSizeX, ChunkSizeY, ChunkSizeZ, GridSize, IsoValue, pos, 8, 2, std::ref(vertices2), std::ref(indices2), std::ref(normals2));
    MCData data3(ChunkSizeX, ChunkSizeY, ChunkSizeZ, GridSize, IsoValue, pos, 8, 3, std::ref(vertices3), std::ref(indices3), std::ref(normals3));

    
    MCData data4(ChunkSizeX, ChunkSizeY, ChunkSizeZ, GridSize, IsoValue, pos, 8, 4, std::ref(vertices4), std::ref(indices4), std::ref(normals4));
    MCData data5(ChunkSizeX, ChunkSizeY, ChunkSizeZ, GridSize, IsoValue, pos, 8, 5, std::ref(vertices5), std::ref(indices5), std::ref(normals5));
    MCData data6(ChunkSizeX, ChunkSizeY, ChunkSizeZ, GridSize, IsoValue, pos, 8, 6, std::ref(vertices6), std::ref(indices6), std::ref(normals6));
    MCData data7(ChunkSizeX, ChunkSizeY, ChunkSizeZ, GridSize, IsoValue, pos, 8, 7, std::ref(vertices7), std::ref(indices7), std::ref(normals7));

    double time1 = glfwGetTime();
    ctpl::thread_pool threadPool(4);

    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data0));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data1));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data2));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data3));

    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data4));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data5));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data6));
    threadPool.push(boost::bind(&MarchingCubes::processSection, (*this),0, data7));
    double time2 = glfwGetTime();

    //std::cout << time2 - time1 << std::endl;


    /*boost::thread t1(boost::bind(&MarchingCubes::processSection, (*this),0, data0));
    boost::thread t2(boost::bind(&MarchingCubes::processSection, (*this),0, data1));
    boost::thread t3(boost::bind(&MarchingCubes::processSection, (*this),0, data2));
    boost::thread t4(boost::bind(&MarchingCubes::processSection, (*this),0, data3));


    boost::thread t5(boost::bind(&MarchingCubes::processSection, (*this),0, data4));
    boost::thread t6(boost::bind(&MarchingCubes::processSection, (*this),0, data5));
    boost::thread t7(boost::bind(&MarchingCubes::processSection, (*this),0, data6));
    boost::thread t8(boost::bind(&MarchingCubes::processSection, (*this),0, data7));*/


    threadPool.stop(true);
    //boost::thread test(boost::bind(&MarchingCubes::processSection, (*this), 0, data0));

    //test.join();

    //t1.join();
    
    for(int i = 0; i < vertices0.size(); i++) {
        vertices.push_back(vertices0[i]);
        normals.push_back(normals0[i]);
    }
    
    //t2.join();
    
    
    for(int i = 0; i < vertices1.size(); i++) {
        vertices.push_back(vertices1[i]);
        normals.push_back(normals1[i]);

    }
    
    //t3.join();
    
    
    for(int i = 0; i < vertices2.size(); i++) {
        vertices.push_back(vertices2[i]);
        normals.push_back(normals2[i]);

    }
    
    //t4.join();
    
    for(int i = 0; i < vertices3.size(); i++) {
        vertices.push_back(vertices3[i]);
        normals.push_back(normals3[i]);

    }
    
    //t5.join();
    
    for(int i = 0; i < vertices4.size(); i++) {
        vertices.push_back(vertices4[i]);
        normals.push_back(normals4[i]);

    }
    
    //t6.join();
    
    for(int i = 0; i < vertices5.size(); i++) {
        vertices.push_back(vertices5[i]);
        normals.push_back(normals5[i]);

    }
    
    //t7.join();
    
    for(int i = 0; i < vertices6.size(); i++) {
        vertices.push_back(vertices6[i]);
        normals.push_back(normals6[i]);

    }
    
    //t8.join();
    double firstTime = glfwGetTime();
    
    
    for(int i = 0; i < vertices7.size(); i++) {
        vertices.push_back(vertices7[i]);
        normals.push_back(normals7[i]);

    }
    
    //t8.detach();
    
    
    double secondTime = glfwGetTime();
    
    //std::reverse(indices.begin(), indices.end());
    //std::cout << secondTime - firstTime << std::endl;
    
    
    for(int i = 0; i < vertices.size(); i ++) {
        indices.push_back(i);
    }
}
