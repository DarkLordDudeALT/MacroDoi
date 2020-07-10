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

#include <iostream>

int main() {
	KeyboardListener::initialize();
	KeyboardActivator::initialize();

	std::vector<int> a;
	a.push_back('A');
	a.push_back('S');
	a.push_back('D');

	std::vector<KeyAction> b;
	b.push_back(KeyAction(VK_BACK, false));
	b.push_back(KeyAction(VK_BACK, false));
	b.push_back(KeyAction(VK_BACK, false));
	b.push_back(KeyAction(VK_SHIFT, false));
	b.push_back(KeyAction('H', false));
	b.push_back(KeyAction('E', false));
	b.push_back(KeyAction('L', false));
	b.push_back(KeyAction('L', false));
	b.push_back(KeyAction('O', false));
	b.push_back(KeyAction(500, true));
	b.push_back(KeyAction(VK_SHIFT, false));
	b.push_back(KeyAction('1', false));

	std::vector<int> c;
	c.push_back('H');
	c.push_back('E');
	c.push_back('L');
	c.push_back('L');
	c.push_back('O');

	std::vector<KeyAction> d;
	d.push_back(KeyAction(VK_BACK, false));
	d.push_back(KeyAction(VK_BACK, false));
	d.push_back(KeyAction(VK_BACK, false));
	d.push_back(KeyAction(VK_BACK, false));
	d.push_back(KeyAction(VK_BACK, false));
	d.push_back(KeyAction(VK_SHIFT, false));
	d.push_back(KeyAction('G', false));
	d.push_back(KeyAction('O', false));
	d.push_back(KeyAction('O', false));
	d.push_back(KeyAction('D', false));
	d.push_back(KeyAction('B', false));
	d.push_back(KeyAction('Y', false));
	d.push_back(KeyAction('E', false));

	std::vector<int> e;
	e.push_back('Z');
	e.push_back('X');
	e.push_back('C');

	std::vector<KeyAction> f;
	f.push_back(KeyAction(VK_RWIN, false));
	f.push_back(KeyAction(200, true));
	f.push_back(KeyAction('C', false));
	f.push_back(KeyAction('M', false));
	f.push_back(KeyAction('D', false));
	f.push_back(KeyAction(200, true));
	f.push_back(KeyAction(VK_RETURN, false));
	f.push_back(KeyAction(400, true));
	f.push_back(KeyAction('S', false));
	f.push_back(KeyAction('H', false));
	f.push_back(KeyAction('U', false));
	f.push_back(KeyAction('T', false));
	f.push_back(KeyAction('D', false));
	f.push_back(KeyAction('O', false));
	f.push_back(KeyAction('W', false));
	f.push_back(KeyAction('N', false));
	f.push_back(KeyAction(VK_SPACE, false));
	f.push_back(KeyAction(VK_OEM_2, false));
	f.push_back(KeyAction('F', false));
	f.push_back(KeyAction(VK_SPACE, false));
	f.push_back(KeyAction(VK_OEM_2, false));
	f.push_back(KeyAction('T', false));
	f.push_back(KeyAction(VK_SPACE, false));
	f.push_back(KeyAction('0', false));
	f.push_back(KeyAction(VK_RETURN, false));

	Macro macro1(new KeyboardActivator(a, 0.2), new KeyboardExecutor(b));
	Macro macro2(new KeyboardActivator(c, 0.4), new KeyboardExecutor(d));
	Macro macro3(new KeyboardActivator(e, 0.4), new KeyboardExecutor(f));


	clock_t lastTime = clock();

	while (true) {
		double deltaTime = (clock() - lastTime) / static_cast<double>(CLOCKS_PER_SEC);
		lastTime = clock();

		KeyboardListener::tick();
		macro1.tick(deltaTime);
		macro2.tick(deltaTime);
		macro3.tick(deltaTime);
	}


	KeyboardActivator::uninitialize();
	KeyboardListener::uninitialize();

	return 0;
}
