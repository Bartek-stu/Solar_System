#ifndef TROJKAT_OBIEKTOWO_VBO_H
#define TROJKAT_OBIEKTOWO_VBO_H

#include <glad/glad.h>


class VBO {
public:
    GLuint ID;

    VBO(GLfloat* vertices, GLsizeiptr size);

    void Update(GLfloat *vertices, GLsizeiptr size);
    void Bind();
    void Unbind();
    void Delete();
};


#endif //TROJKAT_OBIEKTOWO_VBO_H
