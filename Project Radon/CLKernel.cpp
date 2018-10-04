//
// Created by Matthew Bickham on 10/1/18.
//

#include "CLKernel.h"
#include <OpenCL/cl.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>



CLKernel::CLKernel(std::string FilePath) {

    cl_uint platformIdCount = 0;
    clGetPlatformIDs(0, nullptr, &platformIdCount);



    std::vector<cl_platform_id> platformIds (platformIdCount);

    clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);

    cl_uint deviceIdCount = 0;

    clGetDeviceIDs(platformIds[0], CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceIdCount);

    std::vector<cl_device_id> deviceIds(deviceIdCount);

    clGetDeviceIDs(platformIds[0], CL_DEVICE_TYPE_ALL, deviceIdCount, deviceIds.data(), nullptr);

    const cl_context_properties  contextProperties [] = {
            CL_CONTEXT_PLATFORM,
            reinterpret_cast<cl_context_properties > (platformIds[0]),
            0, 0
    };


    int error = 0;

    context = clCreateContext(contextProperties, deviceIdCount, deviceIds.data(), nullptr, nullptr, &error);


    if(error == CL_SUCCESS) {
        std::cout << "WOOT WOOT YEAH" << std::endl;
    }
    //std::cout << error << std::endl;


    FILE *file;

    char *source_str;
    size_t source_size;

    file = fopen("SimplexNoise.cl", "r");

    source_str = (char *) malloc(0x100000);
    source_size = fread(source_str, 1, 0x100000, file);
    fclose(file);


    program = clCreateProgramWithSource(context, 1, (const char **) &source_str, (const size_t *) &source_size, &error);
    //std::cout << error << std::endl;



}

