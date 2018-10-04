//
// Created by Matthew Bickham on 10/1/18.
//

#ifndef PROJECT_RADON_CLKERNEL_H
#define PROJECT_RADON_CLKERNEL_H

#include <string>
#include <OpenCL/cl.h>
class CLKernel {


public:

    CLKernel(std::string FilePath);

    cl_context context;
    cl_program program;
private:


};


#endif //PROJECT_RADON_CLKERNEL_H
