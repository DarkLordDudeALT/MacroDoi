//============================================================================
// Name        : macrodoi.cpp
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Computer macros
//============================================================================

#include <iostream>
#include "keyboard_helper.h"

void functin(KeyEvent& event) {
	std::cout << event.getKey() << ", " << event.getFlags() << std::endl;
}

int main() {
	KeyboardListener::initialize();


	KeyboardListener::registerListener(functin);

	while (true) {
		KeyboardListener::tick();
	}


	KeyboardListener::uninitialize();

	return 0;
}
