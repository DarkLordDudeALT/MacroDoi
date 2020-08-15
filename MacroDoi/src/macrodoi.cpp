//============================================================================
// Name        : macrodoi.cpp
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Computer macros
//============================================================================

#include <chrono>
#include <thread>
#include <time.h>
#include "macro/keyboard_macro.h"
#include "macro_loader.h"

using namespace std;

// TODO Allow instance-based key-listener registration with interfaces.
// TODO Allow macros to invoke other macros. Make a label activator, and an executor to trigger it.

int main() {
	KeyboardListener::initialize();
	KeyboardMacros::initialize();
	MacroLoader::loadMacrosFromFile();


	clock_t lastTime = clock();

	while (true) {
		double deltaTime = (clock() - static_cast<double>(lastTime)) / CLOCKS_PER_SEC;
		lastTime = clock();

		KeyboardListener::tick();
		MacroLoader::tick(deltaTime);

		if (deltaTime < 0.03)
			this_thread::sleep_for(chrono::milliseconds(30 - static_cast<int>(deltaTime / 1000)));
	}


	KeyboardMacros::uninitialize();
	KeyboardListener::uninitialize();
	MacroLoader::unloadMacros();

	return 0;
}
