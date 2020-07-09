#include "keyboard_helper.h"

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
