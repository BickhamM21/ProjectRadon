cmake_minimum_required(VERSION 3.12)
project(Project_Radon)


set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    set(Boost_INCLUDE_DIR C:/mingw-w64/MinGW/include)
    set(Boost_LIBRARY_DIR C:/mingw-w64/MinGW/lib)
endif(WIN32)

find_package(Boost COMPONENTS system filesystem thread)
find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)



include_directories(${PROJECT_SOURCE_DIR}/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/glm)
include_directories(${PROJECT_SOURCE_DIR}/glew/include)
include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/ctpl)


add_subdirectory(${PROJECT_SOURCE_DIR}/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/glew/build/cmake)



set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)


#find_package(Boost COMPONENTS log log_setup thread filesystem system REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIR})

#find_package(Boost REQUIRED)



add_executable(Project_Radon
        "Project Radon/Camera.cpp"
        "Project Radon/Camera.hpp"
        "Project Radon/Entity.cpp"
        "Project Radon/Entity.hpp"
        "Project Radon/main.cpp"
        "Project Radon/MarchingCubes.cpp"
        "Project Radon/MarchingCubes.hpp"
        "Project Radon/Mesh.cpp"
        "Project Radon/Mesh.hpp"
        "Project Radon/Shader.cpp"
        "Project Radon/Shader.hpp"
        "Project Radon/VAO.cpp"
        "Project Radon/VAO.hpp"
        "Project Radon/Window.cpp"
        "Project Radon/Window.hpp"
        "Project Radon/CLKernel.cpp"
        "Project Radon/CLKernel.h"


        )

#target_link_libraries(Project_Radon ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})

target_link_libraries(Project_Radon glfw ${OPENGL_LIBRARY} ${GLFW_LIBRARIES} glew ${GLEW_LIBRARY} ${OpenCL_LIBRARIES} ${Boost_LIBRARIES} )

