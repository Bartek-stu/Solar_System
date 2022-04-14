#ifndef TROJKAT_OBIEKTOWO_VAO_H
#define TROJKAT_OBIEKTOWO_VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
    GLuint ID;
    VAO();

    void LinkVBO(VBO& VBO, GLuint positionLocation, GLuint colorLocation);
    void Bind();
    void Unbind();
    void Delete();

};


#endif //TROJKAT_OBIEKTOWO_VAO_H
