//
// Created by Matthew Bickham on 10/1/18.
//

#include "CLKernel.h"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <vector>
#include <iostream>
#include <string>
#include <fstream>


CLKernel::CLKernel(char *FilePath, char *Kernel) {

    int error;


    error = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    error = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &error);



    if(error == CL_SUCCESS) {
        std::cout << "WOOT WOOT YEAH" << std::endl;
    }
    //std::cout << error << std::endl;


    FILE *file;

    char *source_str;
    size_t source_size;

    file = fopen(FilePath, "r");

    source_str = (char *) malloc(0x100000);
    source_size = fread(source_str, 1, 0x100000, file);
    fclose(file);


    program = clCreateProgramWithSource(context, 1, (const char **) &source_str, (const size_t *) &source_size, &error);
    error = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    kernel = clCreateKernel(program, Kernel, &error);

    std::cout << error << std::endl;



}

