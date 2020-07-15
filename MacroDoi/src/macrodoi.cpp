//============================================================================
// Name        : macrodoi.cpp
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Computer macros
//============================================================================

#include <windows.h>
#include <time.h>
#include "macro/keyboard_macro.h"
#include "macro_loader.h"

int main() {
	KeyboardListener::initialize();
	KeyboardMacros::initialize();
	MacroLoader::loadMacrosFromFile();


	clock_t lastTime = clock();

	while (true) {
		double deltaTime = (clock() - lastTime) / static_cast<double>(CLOCKS_PER_SEC);
		lastTime = clock();

		KeyboardListener::tick();
		MacroLoader::tick(deltaTime);
	}


	KeyboardMacros::uninitialize();
	KeyboardListener::uninitialize();
	MacroLoader::unloadMacros();

	return 0;
}
