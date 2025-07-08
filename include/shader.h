#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
    GLuint program;
    public:
        Shader(std::string vFile, std::string fFile);
        std::string read_file(std::string path);
        void set_mat4(std::string name, glm::mat4 matrix);
        void set_int(std::string name, int value);
        void set_float(std::string name, float value);
        void set_vec3(std::string name, glm::vec3 value);
        void check_compilation(GLuint shader, std::string type);
        void use();
        ~Shader();
};
