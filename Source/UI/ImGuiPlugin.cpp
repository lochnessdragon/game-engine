#include "ImGuiPlugin.h"

// Engine - dependency files
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void ImGuiPlugin::Setup(Window* win) {
	// Setup Imgui Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking (Docking branch Only)

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(win->GetHandle(), true);
	ImGui_ImplOpenGL3_Init();
}

void ImGuiPlugin::Shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiPlugin::StartFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiPlugin::EndFrame() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}