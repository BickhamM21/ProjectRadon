//
//  VAO.hpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/12/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#ifndef VAO_hpp
#define VAO_hpp

#include <stdio.h>

class VAO {
public:
    VAO();
    int bindedAttributes;
    unsigned int VAOID;
    void BindVAO();
    void UnBindVAO();
    void editVertexAttribPointer(int Attrib, int size, int type, int normalized, int stride, const void* pointer);
    void EnableAttrib(int Attrib);
    
private:
    
};


#endif /* VAO_hpp */
