//
//  Light.hpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/24/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp


#include <stdio.h>
#include <glm/glm.hpp>


class Light {
    public:
    
    glm::vec3 pos;
    float intensity;
    glm::vec3 color;
    Light(glm::vec3 pos, float intensity, glm::vec3 color);
    
    private:
    
    
};

#endif /* Light_hpp */
