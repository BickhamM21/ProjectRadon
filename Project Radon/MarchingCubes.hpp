//
//  MarchingCubes.hpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/13/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#ifndef MarchingCubes_hpp
#define MarchingCubes_hpp

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <thread>
#include <ctpl.h>

#include "CLKernel.h"

struct MCData{
    int ChunkSizeX, ChunkSizeY, ChunkSizeZ, threadID, numThreads;
    float gridSize, isoValue;
    glm::vec3 pos;
    std::vector<glm::vec3> &Vertices;
    std::vector<int> &Indices;
    std::vector<glm::vec3> &Normals;

    
    
    MCData(int chunkSizeX, int chunkSizeY, int chunkSizeZ, float GridSize, float IsoValue, glm::vec3 Pos, int NumThreads, int ThreadID, std::vector<glm::vec3> &vertices, std::vector<int> &indices, std::vector<glm::vec3> &normals)
    :Vertices(vertices), Indices(indices), ChunkSizeX(chunkSizeX), ChunkSizeY(chunkSizeY), ChunkSizeZ(chunkSizeZ), gridSize(GridSize), isoValue(IsoValue), pos(Pos), threadID(ThreadID), numThreads(NumThreads), Normals(normals)
    {
        
    }


    
};


class MarchingCubes {
    
public:
    int ChunkSizeX;
    int ChunkSizeY;
    int ChunkSizeZ;
    float GridSize;
    float IsoValue;


    MarchingCubes(int chunkSizeX, int chunkSize, int chunkSizeZ, float gridSize, float isoValue, int numThreads,
                  glm::vec3 pos, CLKernel Kernel);
    std::vector<glm::vec3> vertices;
    std::vector<int> indices;
    std::vector<glm::vec3> normals;
    
    
    glm::vec3 VertexInterp(float IsoValue, glm::vec3 p1, glm::vec3 p2, float v1, float v2);
    glm::vec3 NormalCalc(glm::vec3 pos);
    
    float isoValues[35973];
    
    float Circle(glm::vec3 pos);
    float smin(float a, float b, float k);
    
    float* voxels;
    
    void processSection(int id, MCData data);
    
    void UpdateMesh();
    float VerticesTime;
    
    glm::vec3 pos;

    ctpl::thread_pool* tp = new ctpl::thread_pool(4);


    CLKernel kernel;

private:
    
    
};

#endif /* MarchingCubes_hpp */
