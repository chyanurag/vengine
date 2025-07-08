#include "shader.h"

Shader::Shader(std::string vFile, std::string fFile) {
    std::string vString = read_file(vFile);
    std::string fString = read_file(fFile);

    const char* vSource = vString.c_str();
    const char* fSource = fString.c_str();

    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vSource, NULL);
    glCompileShader(vertex);
    check_compilation(vertex, "vertex");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fSource, NULL);
    glCompileShader(fragment);
    check_compilation(fragment, "fragment");

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

std::string Shader::read_file(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file " << path << '\n';
        throw "Error opening file\n";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void Shader::check_compilation(GLuint shader, std::string type) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::cerr << type << " shader compilation failed\n" << infoLog << '\n';
        throw "shader compilation failed\n";
    }
}

void Shader::set_mat4(std::string name, glm::mat4 matrix) {
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_int(std::string name, int value) {
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, value);
}

void Shader::set_float(std::string name, float value) {
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform1f(location, value);
}

void Shader::set_vec3(std::string name, glm::vec3 value) {
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::use() {
    glUseProgram(program);
}

Shader::~Shader() {
    glDeleteProgram(program);
}
