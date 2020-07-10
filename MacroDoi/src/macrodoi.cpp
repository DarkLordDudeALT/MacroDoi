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
	b.push_back(KeyAction(VK_BACK, 0b00));
	b.push_back(KeyAction(VK_BACK, 0b00));
	b.push_back(KeyAction(VK_BACK, 0b00));
	b.push_back(KeyAction(VK_SHIFT, 0b00));
	b.push_back(KeyAction(L'こ', 0b10));
	b.push_back(KeyAction(L'ん', 0b10));
	b.push_back(KeyAction(L'に', 0b10));
	b.push_back(KeyAction(L'ち', 0b10));
	b.push_back(KeyAction(L'わ', 0b10));
	b.push_back(KeyAction(500, 0b01));
	b.push_back(KeyAction(VK_SHIFT, 0b00));
	b.push_back(KeyAction('1', 0b00));

	std::vector<int> c;
	c.push_back('H');
	c.push_back('E');
	c.push_back('L');
	c.push_back('L');
	c.push_back('O');

	std::vector<KeyAction> d;
	d.push_back(KeyAction(VK_BACK, 0b00));
	d.push_back(KeyAction(VK_BACK, 0b00));
	d.push_back(KeyAction(VK_BACK, 0b00));
	d.push_back(KeyAction(VK_BACK, 0b00));
	d.push_back(KeyAction(VK_BACK, 0b00));
	d.push_back(KeyAction(VK_SHIFT, 0b00));
	d.push_back(KeyAction('G', 0b00));
	d.push_back(KeyAction('O', 0b00));
	d.push_back(KeyAction('O', 0b00));
	d.push_back(KeyAction('D', 0b00));
	d.push_back(KeyAction('B', 0b00));
	d.push_back(KeyAction('Y', 0b00));
	d.push_back(KeyAction('E', 0b00));

	std::vector<int> e;
	e.push_back('Z');
	e.push_back('X');
	e.push_back('C');

	std::vector<KeyAction> f;
	f.push_back(KeyAction(VK_RWIN, 0b00));
	f.push_back(KeyAction(200, 0b01));
	f.push_back(KeyAction('C', 0b00));
	f.push_back(KeyAction('M', 0b00));
	f.push_back(KeyAction('D', 0b00));
	f.push_back(KeyAction(200, 0b01));
	f.push_back(KeyAction(VK_RETURN, 0b00));
	f.push_back(KeyAction(500, 0b01));
	f.push_back(KeyAction('S', 0b00));
	f.push_back(KeyAction('H', 0b00));
	f.push_back(KeyAction('U', 0b00));
	f.push_back(KeyAction('T', 0b00));
	f.push_back(KeyAction('D', 0b00));
	f.push_back(KeyAction('O', 0b00));
	f.push_back(KeyAction('W', 0b00));
	f.push_back(KeyAction('N', 0b00));
	f.push_back(KeyAction(VK_SPACE, 0b00));
	f.push_back(KeyAction(VK_OEM_2, 0b00));
	f.push_back(KeyAction('S', 0b00));
	f.push_back(KeyAction(VK_SPACE, 0b00));
	f.push_back(KeyAction(VK_OEM_2, 0b00));
	f.push_back(KeyAction('F', 0b00));
	f.push_back(KeyAction(VK_SPACE, 0b00));
	f.push_back(KeyAction(VK_OEM_2, 0b00));
	f.push_back(KeyAction('T', 0b00));
	f.push_back(KeyAction(VK_SPACE, 0b00));
	f.push_back(KeyAction('0', 0b00));
	f.push_back(KeyAction(VK_RETURN, 0b00));

	Macro macro1(new KeyboardActivator(a, 0.2), new KeyboardExecutor(b));
	Macro macro2(new KeyboardActivator(c, 0.4), new KeyboardExecutor(d));
	Macro macro3(new KeyboardActivator(e, 0.1), new KeyboardExecutor(f));


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
