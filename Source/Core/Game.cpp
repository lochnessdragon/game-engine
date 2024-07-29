#include "Game.h"

// Engine - dependency files
#include <GLFW/glfw3.h>

// Engine files
#include <Core/Input.h>
#include <UI/ImGuiPlugin.h>

Game* Game::Instance = nullptr;

Game::Game(GameSpec spec) {
	// also sets up input
	MainWindow = std::make_shared<Window>(spec.name, spec.width, spec.height);
	
	Input::Get()->GetKeyEventHandler().AddListener([this](auto keyData) {
		if (keyData.key == GLFW_KEY_ESCAPE && keyData.action == GLFW_PRESS) {
			MainWindow->SetShouldClose();
			return true;
		}
		return false;
	});

	LastFrameTime = glfwGetTime();

	ImGuiPlugin::Setup(MainWindow.get());

	Game::Instance = this;
}

Game::~Game() {
	ImGuiPlugin::Shutdown();
}

void Game::MainLoop() {
	float now = glfwGetTime();
	float delta_t = now - LastFrameTime;
	LastFrameTime = now;

	Update(delta_t);

	// imgui callbacks
	ImGuiPlugin::StartFrame();
	OnImGui();
	ImGuiPlugin::EndFrame();

	// end of frame, swap framebuffers and update the input
	MainWindow->Swap();
	Input::Get()->Update();
}