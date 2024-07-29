#pragma once

#include <string>
#include <unordered_map>
#include <core/platform.h>

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderVariable {
    GLint location;
    GLenum type;
    GLint size;
};

class Shader {
private:
    GLuint handle;
    std::unordered_map<std::string, ShaderVariable> uniformVars;
    std::unordered_map<std::string, ShaderVariable> attribs;

    GLuint CompileShader(GLenum type, const std::string& filename, const char* source);
public:
    Shader(const std::string& vertFile, const std::string& fragFile);
    ~Shader();

    void Use();
    
    GLint GetUniformLocation(const char* name) { return glGetUniformLocation(this->handle, name); }

    void LoadUniform(const std::string& name, glm::mat3 matrix);
    void LoadUniform(const std::string& name, glm::mat4 matrix);
    void LoadUniform(const std::string& name, int value); // { glUniform1i(location, value); }
    void LoadUniform(const std::string& name, float value); // { glUniform1f(location, value); }
    void LoadUniform(const std::string& name, glm::vec2 value); // { glUniform2fv(location, 1, glm::value_ptr(value)); }
    void LoadUniform(const std::string& name, glm::vec3 value); // { glUniform3fv(location, 1, glm::value_ptr(value)); }
    void LoadUniform(const std::string& name, glm::vec4 value);
    void LoadUniformArray(const std::string& name, int count, int* values);
};
