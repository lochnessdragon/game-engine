#pragma once

#include <cstdint>
#include <thread>
#include <optional>

#include "Private/CliWorker.h"

class Terminal {
private:
	static uint8_t INSTANCE_COUNT; // should only ever be 0 or 1.

	bool first = false; // whether this is the first instance created.

	cli::Private::CliWorker worker; // cli rendering worker thread representation
public:
	Terminal(); // start the terminal worker thread
	~Terminal(); // shutdown the terminal worker thread
};