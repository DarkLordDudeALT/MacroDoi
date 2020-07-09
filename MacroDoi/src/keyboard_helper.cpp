//============================================================================
// Name        : keyboard_helper.cpp
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Implementation of keyboard_helper.h
//============================================================================

#include "keyboard_helper.h"

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <windows.h>

bool isKeyPressed(int virtualKey) {
	return GetAsyncKeyState(virtualKey) & 0x8000;
}

bool isKeyToggled(int virtualKey) {
	return GetAsyncKeyState(virtualKey) & 0x0001;
}



void pressVKey(int virtualKey, bool isUnicode) {
	KEYBDINPUT keyPress;

	keyPress.time = 0;
	keyPress.dwExtraInfo = GetMessageExtraInfo();

	if (isUnicode) {
		keyPress.wVk = 0;
		keyPress.wScan = virtualKey;
		keyPress.dwFlags = KEYEVENTF_UNICODE;

	} else {
		keyPress.wVk = virtualKey;
		keyPress.dwFlags = 0;
	}


	INPUT keyInput;
	keyInput.type = INPUT_KEYBOARD;
	keyInput.ki = keyPress;
	INPUT* keyInputPointer = &keyInput;

	SendInput(1, keyInputPointer, sizeof(INPUT));

	keyPress.dwFlags |= KEYEVENTF_KEYUP;
	keyPress.dwExtraInfo = GetMessageExtraInfo();

	keyInput.ki = keyPress;

	SendInput(1, keyInputPointer, sizeof(INPUT));
}

void holdVKey(int virtualKey, bool isUnicode) {
	KEYBDINPUT keyHold;

	keyHold.time = 0;
	keyHold.dwExtraInfo = GetMessageExtraInfo();

	if (isUnicode) {
		keyHold.wVk = 0;
		keyHold.wScan = virtualKey;
		keyHold.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_UNICODE;

	} else {
		keyHold.wVk = virtualKey;
		keyHold.dwFlags = KEYEVENTF_EXTENDEDKEY;
	}

	INPUT keyInput;
	keyInput.type = INPUT_KEYBOARD;
	keyInput.ki = keyHold;

	SendInput(1, &keyInput, sizeof(INPUT));
}

void releaseVKey(int virtualKey, bool isUnicode) {
	KEYBDINPUT keyRelease;

	keyRelease.time = 0;
	keyRelease.dwExtraInfo = GetMessageExtraInfo();

	if (isUnicode) {
		keyRelease.wVk = 0;
		keyRelease.wScan = virtualKey;
		keyRelease.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY | KEYEVENTF_UNICODE;

	} else {
		keyRelease.wVk = virtualKey;
		keyRelease.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY;
	}

	INPUT keyInput;
	keyInput.type = INPUT_KEYBOARD;
	keyInput.ki = keyRelease;

	SendInput(1, &keyInput, sizeof(INPUT));
}



KeyEvent::KeyEvent(int virtualKey, short flags) {
	key = virtualKey;
	this->flags = flags;
}

int KeyEvent::getKey() {
	return key;
}

short KeyEvent::getFlags() {
	return flags;
}



static bool initialized = false;
static std::vector<KeyListener> keyListeners = std::vector<KeyListener>();
static SHORT* previousKeyboard;

void KeyboardListener::initialize() {
	if (initialized)
		throw std::logic_error("KeyboardListener is already initialized!");

	initialized = true;
	previousKeyboard = new SHORT[256];

	for (int i = 0; i < 256; i++)
		previousKeyboard[i] = GetAsyncKeyState(i);
}

void KeyboardListener::uninitialize() {
	if (!initialized)
		throw std::logic_error("KeyboardListener is not initialized!");

	initialized = false;
	delete[] previousKeyboard;
}

void KeyboardListener::tick() {
	if (keyListeners.size())
		// Looks for changes in the keys between last execution and now.
		for (int i = 0; i < 256; i++) {
			SHORT keyState = GetAsyncKeyState(i);
			SHORT* previousKeyState = &previousKeyboard[i];

			if (keyState != *previousKeyState) {
				*previousKeyState = keyState;
				KeyEvent keyEvent(i, keyState);

				for (KeyListener listener : keyListeners)
					listener(keyEvent);
			}
		}
}

void KeyboardListener::registerListener(KeyListener listener) {
	keyListeners.push_back(listener);
}

void KeyboardListener::unregisterListener(KeyListener listener) {
	auto endIterator = keyListeners.end();
	auto result = std::find(keyListeners.begin(), endIterator, listener);

	if (result != endIterator)
		keyListeners.erase(result);
}
