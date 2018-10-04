//
// Created by Matthew Bickham on 10/1/18.
//

#include "CLKernel.h"
#include <OpenCL/cl.h>
#include <vector>
#include <iostream>
#include <string>

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

    cl_context context = clCreateContext(contextProperties, deviceIdCount, deviceIds.data(), nullptr, nullptr, &error);


    if(error == CL_SUCCESS) {
        std::cout << "WOOT WOOT YEAH" << std::endl;
    }
    //std::cout << error << std::endl;

    

}

