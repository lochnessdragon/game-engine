#pragma once

#ifdef _WIN32
#include "Platform/Windows/WindowsTerminal.h"
using Terminal = Platform::Windows::WindowsTerminal;
#else
#include "Platform/Unix/UnixTerminal.h"
using Terminal = Platform::Unix::UnixTerminal;
#endif