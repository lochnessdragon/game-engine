#pragma once

//#include <filesystem>
#include <iostream>
#include <memory>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

// engine files
#include "Log.h"

// project files
#include "Game.h"

extern Game* CreateGame();