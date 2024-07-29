#pragma once

// System files
#include <memory>

// Engine - dependency files
#include <imgui.h>

// Engine files
#include <core/window.h>

namespace ImGuiPlugin {
	void Setup(Window* win);
	void Shutdown();

	void StartFrame();
	void EndFrame();
}