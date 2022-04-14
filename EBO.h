#ifndef TROJKAT_OBIEKTOWO_EBO_H
#define TROJKAT_OBIEKTOWO_EBO_H

#include <glad/glad.h>

class EBO {
public:
    GLuint ID;
    EBO(GLuint* indices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};


#endif //TROJKAT_OBIEKTOWO_EBO_H
