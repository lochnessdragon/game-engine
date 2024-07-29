#include "EntryPoint.h"

#include <GLFW/glfw3.h>

Game* game = nullptr;

void game_loop() {
	game->MainLoop();
}

int main() {
	try {
		Log::Init();
		Log::GetGameLog()->info("Game startup...");
		game = CreateGame();

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(game_loop, 0, false);
#else
		while (!game->MainWindow->ShouldClose()) {
			game->MainLoop();
		}
#endif

		return 0;
	}
	catch (std::exception& e) {
		std::cout << "FATAL ERROR: " << e.what() << std::endl;
	}

	return 1;
}
