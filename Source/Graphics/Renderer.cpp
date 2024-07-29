#include "renderer.h"

#include <core/platform.h>
#include "GLAPI.h"

#include <core/log.h>

Renderer::Renderer() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    wireframeOn = false;
}

void Renderer::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setClearColor(float red, float green, float blue, float alpha) {
	glClearColor(red, green, blue, alpha);
}

void Renderer::setClearColor(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::startFrame() {
	this->clear();
}

void Renderer::render(std::shared_ptr<Camera> camera, std::shared_ptr<Object> object, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, const AmbientLight& ambientLight, const Light& light) {
	//Log::getRendererLog()->trace("Rendering obj");
    
    // calculate normal mat
    glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(camera->getViewMat() * object->getModelMat())));
    
    // set up shaders
	shader->Use();
	shader->LoadUniform("modelMat", object->getModelMat());
	shader->LoadUniform("viewMat", camera->getViewMat());
	shader->LoadUniform("projMat", camera->getProjMat());
    shader->LoadUniform("normalMat", normalMat);
	shader->LoadUniform("texture0", 0);
    
    // lighting settings
    shader->LoadUniform("ambientLightColor", ambientLight.color);
    shader->LoadUniform("ambientLightStrength", ambientLight.intensity);
    shader->LoadUniform("lightPos", light.position);
    shader->LoadUniform("lightColor", light.color);
    shader->LoadUniform("lightStrength", light.intensity);
    shader->LoadUniform("specularStrength", light.intensity);
	
	glActiveTexture(GL_TEXTURE0);
	texture->Bind();
	mesh->Bind();
	glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
	mesh->Unbind();
}

void Renderer::toggleWireframe() {
    if(wireframeOn) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wireframeOn = false;
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireframeOn = true;
    }
}
