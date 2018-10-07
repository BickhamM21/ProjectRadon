//
// Created by Matthew Bickham on 10/1/18.
//

#ifndef PROJECT_RADON_CLKERNEL_H
#define PROJECT_RADON_CLKERNEL_H

#include <string>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
class CLKernel {


public:

    CLKernel(std::string FilePath);

    cl_context context;
    cl_program program;
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;


    cl_kernel kernel;


private:


};


#endif //PROJECT_RADON_CLKERNEL_H
