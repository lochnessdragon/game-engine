#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"

class Object {
private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    Mesh& mesh;

    glm::mat4 modelMat;

    bool dirty;

protected:
    void calculateModelMat();

public:
    Object(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, Mesh& mesh);
    ~Object();

    // this method should really be called at the end of a tick, not the beginning
    void tick() { if(dirty) { calculateModelMat(); dirty = false; } }

    void setDirty() { dirty = true; }

    glm::vec3 getPosition() { return position; }
    glm::vec3 getScale() { return scale; }
    glm::vec3 getRotation() { return rotation; }

    void setPosition(glm::vec3 position) { this->position = position; setDirty(); }
    void setScale(glm::vec3 scale) { this->scale = scale; setDirty(); }
    void setRotation(glm::vec3 rotation) { this->rotation = rotation; setDirty(); }

    glm::mat4 getModelMat() { return modelMat; }
};