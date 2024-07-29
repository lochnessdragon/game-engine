#pragma once

#include "Window.h"

struct GameSpec {
	std::string name;
	int width;
	int height;
};

class Game {
public:
	Game(GameSpec spec);
	~Game();

	/**
	* Called by the game loop. This should **not** be overloaded.
	*/
	void MainLoop();

	/**
	* Called by main_loop. Update the position of any objects in this method
	*/
	virtual void Update(float delta_t) = 0;

	/**
	* Callback function bounded by `ImGui::StartFrame` and `ImGui::EndFrame`
	*/
	virtual void OnImGui() {};

	static Game* Get() { return Instance; };

	std::shared_ptr<Window> MainWindow;
	std::unique_ptr<Input> Input; // TODO: Move input to this pointer, not the one stored in Window.

private:
	static Game* Instance;

	float LastFrameTime;
};