#pragma once

#include <core/platform.h>

#include <vector>
#include <initializer_list>

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

struct VertexBufferLayout {
    GLenum dataType;
    int memberCount;
    GLuint dataLength;
    void* data;
};

struct VertexBufferDataType {
    GLenum type;
    int count;
};

enum class MeshUsage {
    // Optimize mesh for set once, read multiple
    Static,
    // Optimize mesh for set multiple, read multiple
    Dynamic
};

class Mesh {
private:
    GLuint handle;

    // buffer handles
    GLuint indicesBuffer;
    std::vector<GLuint> vbos;

    GLuint vertexCount;
    GLuint indexCount;

    GLenum usage;

    static int getDataSize(GLenum dataType);
public:
    Mesh(MeshUsage usage);
    Mesh(const GLuint indices_length, const unsigned int* indices, const GLuint vertices_length, const float* vertices, const GLuint uvs_length, const float* uvs, MeshUsage usage = MeshUsage::Static);
    ~Mesh();

    GLuint AttachBuffer(GLenum dataType, int memberCount, GLuint dataLength, void* data);
    GLuint AttachBuffer(std::initializer_list<VertexBufferDataType> members, GLuint dataLength, void* data);

    void SetIndexBuffer(const GLuint indices_length, const unsigned int* indices);
    void SetBuffer(uint32_t bufferIdx, const GLuint bufferSize, const void* data);

    void Bind();
    void Unbind();

    GLuint GetVertexCount() { return vertexCount; }
    GLuint GetIndexCount() { return indexCount;  }

    // I believe the driver won't give us zeros for buffers, but I really don't know.
    bool IsIndexed() { return indicesBuffer != 0; }
};
