#ifndef TROJKAT_OBIEKTOWO_SHADERCLASS_H
#define TROJKAT_OBIEKTOWO_SHADERCLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>

std::string get_file_contents(const char* filename);


class Shader {
public:
    GLuint ID;
    Shader(const char* vertexFile, const char* fragmentFile);

    void Activate();
    void Delete();
};


#endif //TROJKAT_OBIEKTOWO_SHADERCLASS_H
