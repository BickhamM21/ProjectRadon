//
//  VAO.cpp
//  Project Radon
//
//  Created by Matthew Bickham on 9/12/18.
//  Copyright © 2018 Matthew Bickham. All rights reserved.
//

#include "VAO.hpp"
#include <GL/glew.h>

VAO :: VAO() {
    glGenVertexArrays(1, &VAOID);
    bindedAttributes = 0;
}

void VAO :: BindVAO() {
    glBindVertexArray(VAOID);
}

void VAO :: UnBindVAO() {
    glBindVertexArray(0);
}

void VAO :: editVertexAttribPointer(int Attrib, int size, int type, int normalized, int stride, const void* pointer) {
    
    glVertexAttribPointer(Attrib, size, type, normalized, stride, pointer);

}

void VAO :: EnableAttrib(int Attrib) {
    glEnableVertexAttribArray(Attrib);
}
