#pragma once

/**
* mesh_generator.h:
*	contains procedures to provide meshes of varying standard quality with vertices, indices, and uv coordinates.
*	plans for model loading in here.
*/

#include <memory>
#include <string>

#include <Graphics/Mesh.h>

std::shared_ptr<Mesh> CreateQuad();
std::shared_ptr<Mesh> CreateCube();
std::shared_ptr<Mesh> CreateUVSphere(int slices, int stacks);
std::shared_ptr<Mesh> CreateIcoSphere(int subdivisions);

std::shared_ptr<Mesh> LoadObj(const std::string& filename);
std::shared_ptr<Mesh> LoadGLTF(const std::string& filename);
