#include "Mesh.h"

#include <numeric>

#include <Core/Log.h>

int Mesh::getDataSize(GLenum type) {
    switch (type) {
    case GL_FLOAT:
        return sizeof(float);
    default:
        throw std::runtime_error("Unknown data size for provided GLenum type.");
    }
}

Mesh::Mesh(MeshUsage usage) {
    if (usage == MeshUsage::Static) {
        this->usage = GL_STATIC_DRAW;
    }
    else {
        this->usage = GL_DYNAMIC_DRAW;
    }

    indicesBuffer = 0;

    glGenVertexArrays(1, &this->handle);
    this->Bind();
}

Mesh::Mesh(const GLuint indices_length, const unsigned int* indices, const GLuint vertices_length, const float* vertices, const GLuint uvs_length, const float* uvs, MeshUsage usage) : Mesh(usage) {
    // vertices
    uint32_t floats_per_vertices = 3;
    if (vertices_length == uvs_length) {
        // we need a way to specify that a specific vertex info is 2d and not 3d. This is the best way I have come up with
        // to specify. (if the length of the vertices is equal to the length of the uvs, the vertices must take up 2 floats, since uvs are 2 floats
        floats_per_vertices = 2;
    }
    this->AttachBuffer(GL_FLOAT, floats_per_vertices, vertices_length, (void*)vertices);
    vertexCount = vertices_length / (floats_per_vertices * sizeof(float));

    // uvs
    this->AttachBuffer(GL_FLOAT, 2, uvs_length, (void*) uvs);

    // indices
    this->SetIndexBuffer(indices_length, indices);

    indexCount = indices_length / sizeof(int);
}

GLuint Mesh::AttachBuffer(GLenum dataType, int memberCount, GLuint dataLength, void* data) {
    this->Bind();

    GLuint newIndex = (GLuint) this->vbos.size();

    GLuint newBuffer = 0;
    glGenBuffers(1, &newBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newBuffer);
    glBufferData(GL_ARRAY_BUFFER, dataLength, data, this->usage);
    glEnableVertexAttribArray(newIndex);
    glVertexAttribPointer(newIndex, memberCount, dataType, GL_FALSE, memberCount * getDataSize(dataType), (void*) 0);
    this->vbos.push_back(newBuffer);

    return newIndex;
}

GLuint Mesh::AttachBuffer(std::initializer_list<VertexBufferDataType> members, GLuint dataLength, void* data) {
    this->Bind();

    GLuint newIndex = (GLuint) this->vbos.size();

    GLuint newBuffer = 0;
    glGenBuffers(1, &newBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newBuffer);
    glBufferData(GL_ARRAY_BUFFER, dataLength, data, this->usage);

    GLsizei stride = 0;
    for (const VertexBufferDataType& member : members) {
        stride += member.count * getDataSize(member.type);
    }

    uint32_t location = 0;
    uint32_t accumulatedOffset = 0;
    for (const VertexBufferDataType& member : members) {
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, member.count, member.type, GL_FALSE, stride, (void*) accumulatedOffset);
        accumulatedOffset += member.count * getDataSize(member.type);
        location++;
    }

    this->vbos.push_back(newBuffer);

    return newIndex;
}

void Mesh::SetIndexBuffer(const GLuint indices_length, const unsigned int* indices) {
    this->Bind();

    if (indicesBuffer == 0) {
        glGenBuffers(1, &this->indicesBuffer);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length, indices, this->usage);
}

void Mesh::SetBuffer(uint32_t bufferIdx, const GLuint bufferSize, const void* data) {
    GLuint bufferId = vbos[bufferIdx];
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, data);
}

Mesh::~Mesh() {
    
}

void Mesh::Bind() {
    glBindVertexArray(this->handle);
}

void Mesh::Unbind() {
    glBindVertexArray(0);
}
