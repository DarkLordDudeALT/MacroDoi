//============================================================================
// Name        : keyboard_macro.cpp
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Implements keyboard_macro.h
//============================================================================

#include "keyboard_macro.h"

#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <windows.h>
#include "../macro_loader.h"

static std::vector<KeyboardActivator*> loadedActivators = std::vector<KeyboardActivator*>();
static bool initialized = false;



// TODO Make or find a string hasher and use it to shove a switch into getKeyFromString().
/*
 * Converts the code-name of a key into its key code.
 * Returns -1 if no mapping was found.
 *
 * Pre: None.
 *
 * Post: The mapped key code is returned, or -1.
 */
static int getKeyFromString(std::string& string) {
	// Why must you be like this C++?
	if (string == "CANCEL")
		return VK_CANCEL;
	if (string == "BACK")
		return VK_BACK;
	if (string == "TAB")
		return VK_TAB;
	if (string == "CLEAR")
		return VK_CLEAR;
	if (string == "ENTER")
		return VK_RETURN;
	if (string == "SHIFT")
		return VK_SHIFT;
	if (string == "CONTROL")
		return VK_CONTROL;
	if (string == "ALT")
		return VK_MENU;
	if (string == "PAUSE")
		return VK_PAUSE;
	if (string == "CAPS_LOCK")
		return VK_CAPITAL;
	if (string == "KANA")
		return VK_KANA;
	if (string == "HANGUL")
		return VK_HANGUL;
	if (string == "JUNJA")
		return VK_JUNJA;
	if (string == "FINAL")
		return VK_FINAL;
	if (string == "HANJA")
		return VK_HANJA;
	if (string == "KANJA")
		return VK_KANJI;
	if (string == "ESCAPE")
		return VK_ESCAPE;
	if (string == "CONVERT")
		return VK_CONVERT;
	if (string == "NONCONVERT")
		return VK_NONCONVERT;
	if (string == "ACCEPT")
		return VK_ACCEPT;
	if (string == "MODECHANGE")
		return VK_MODECHANGE;
	if (string == "SPACE")
		return VK_SPACE;
	if (string == "PAGE_UP")
		return VK_PRIOR;
	if (string == "PAGE_DOWN")
		return VK_NEXT;
	if (string == "END")
		return VK_END;
	if (string == "HOME")
		return VK_HOME;
	if (string == "LEFT_ARROW")
		return VK_UP;
	if (string == "UP_ARROW")
		return VK_UP;
	if (string == "RIGHT_ARROW")
		return VK_RIGHT;
	if (string == "DOWN_ARROW")
		return VK_DOWN;
	if (string == "SELECT")
		return VK_SELECT;
	if (string == "PRINT")
		return VK_PRINT;
	if (string == "EXECUTE")
		return VK_EXECUTE;
	if (string == "PRINT_SCREEN")
		return VK_SNAPSHOT;
	if (string == "INSERT")
		return VK_INSERT;
	if (string == "DELETE")
		return VK_DELETE;
	if (string == "HELP")
		return VK_HELP;
	if (string == "0" ||
		string == "1" ||
		string == "2" ||
		string == "3" ||
		string == "4" ||
		string == "5" ||
		string == "6" ||
		string == "7" ||
		string == "8" ||
		string == "9" ||
		string == "0" ||
		string == "A" ||
		string == "B" ||
		string == "C" ||
		string == "D" ||
		string == "E" ||
		string == "F" ||
		string == "G" ||
		string == "H" ||
		string == "I" ||
		string == "J" ||
		string == "K" ||
		string == "L" ||
		string == "M" ||
		string == "N" ||
		string == "O" ||
		string == "P" ||
		string == "Q" ||
		string == "R" ||
		string == "S" ||
		string == "T" ||
		string == "U" ||
		string == "V" ||
		string == "W" ||
		string == "X" ||
		string == "Y" ||
		string == "Z")
		return static_cast<int>(string[0]);
	if (string == "L_WINDOWS")
		return VK_LWIN;
	if (string == "R_WINDOWS")
		return VK_RWIN;
	if (string == "APPS")
		return VK_APPS;
	if (string == "SLEEP")
		return VK_SLEEP;
	if (string == "NUMPAD_0")
		return VK_NUMPAD0;
	if (string == "NUMPAD_1")
		return VK_NUMPAD1;
	if (string == "NUMPAD_2")
		return VK_NUMPAD2;
	if (string == "NUMPAD_3")
		return VK_NUMPAD3;
	if (string == "NUMPAD_4")
		return VK_NUMPAD4;
	if (string == "NUMPAD_5")
		return VK_NUMPAD5;
	if (string == "NUMPAD_6")
		return VK_NUMPAD6;
	if (string == "NUMPAD_7")
		return VK_NUMPAD7;
	if (string == "NUMPAD_8")
		return VK_NUMPAD8;
	if (string == "NUMPAD_9")
		return VK_NUMPAD9;
	if (string == "MULTIPLY")
		return VK_MULTIPLY;
	if (string == "ADD")
		return VK_ADD;
	if (string == "SEPARATOR")
		return VK_SEPARATOR;
	if (string == "SUBTRACT")
		return VK_SUBTRACT;
	if (string == "DECIMAL")
		return VK_DECIMAL;
	if (string == "DIVIDE")
		return VK_DIVIDE;
	if (string == "F1")
		return VK_F1;
	if (string == "F2")
		return VK_F2;
	if (string == "F3")
		return VK_F3;
	if (string == "F4")
		return VK_F4;
	if (string == "F5")
		return VK_F5;
	if (string == "F6")
		return VK_F6;
	if (string == "F7")
		return VK_F7;
	if (string == "F8")
		return VK_F8;
	if (string == "F9")
		return VK_F9;
	if (string == "F10")
		return VK_F10;
	if (string == "F11")
		return VK_F11;
	if (string == "F12")
		return VK_F12;
	if (string == "F13")
		return VK_F13;
	if (string == "F14")
		return VK_F14;
	if (string == "F15")
		return VK_F15;
	if (string == "F16")
		return VK_F16;
	if (string == "F17")
		return VK_F17;
	if (string == "F18")
		return VK_F18;
	if (string == "F19")
		return VK_F19;
	if (string == "F20")
		return VK_F20;
	if (string == "F21")
		return VK_F21;
	if (string == "F22")
		return VK_F22;
	if (string == "F23")
		return VK_F23;
	if (string == "F24")
		return VK_F24;
	if (string == "NUMBER_LOCK")
		return VK_NUMLOCK;
	if (string == "SCROLL")
		return VK_SCROLL;
	if (string == "LEFT_SHIFT")
		return VK_LSHIFT;
	if (string == "RIGHT_SHIFT")
		return VK_RSHIFT;
	if (string == "LEFT_CONTROL")
		return VK_LCONTROL;
	if (string == "RIGHT_CONTROL")
		return VK_RCONTROL;
	if (string == "LEFT_ALT")
		return VK_LMENU;
	if (string == "RIGHT_ALT")
		return VK_RMENU;
	if (string == "BROWSER_BACK")
		return VK_BROWSER_BACK;
	if (string == "BROWSER_FORWARD")
		return VK_BROWSER_FORWARD;
	if (string == "BROWSER_REFRESH")
		return VK_BROWSER_REFRESH;
	if (string == "BROWSER_STOP")
		return VK_BROWSER_STOP;
	if (string == "BROWSER_SEARCH")
		return VK_BROWSER_SEARCH;
	if (string == "BROWSER_FAVORITES")
		return VK_BROWSER_FAVORITES;
	if (string == "BROWSER_HOME")
		return VK_BROWSER_HOME;
	if (string == "VOLUME_MUTE")
		return VK_VOLUME_MUTE;
	if (string == "VOLUME_DOWN")
		return VK_VOLUME_DOWN;
	if (string == "VOLUME_UP")
		return VK_VOLUME_UP;
	if (string == "NEXT_TRACK")
		return VK_MEDIA_NEXT_TRACK;
	if (string == "PREVIOUS_TRACK")
		return VK_MEDIA_PREV_TRACK;
	if (string == "MEDIA_STOP")
		return VK_MEDIA_STOP;
	if (string == "PLAY_PAUSE")
		return VK_MEDIA_PLAY_PAUSE;
	if (string == "LAUNCH_MAIL")
		return VK_LAUNCH_MAIL;
	if (string == "MEDIA_SELECT")
		return VK_LAUNCH_MEDIA_SELECT;
	if (string == "LAUNCH_APP1")
		return VK_LAUNCH_APP1;
	if (string == "LAUNCH_APP2")
		return VK_LAUNCH_APP2;
	if (string == "OEM_1")
		return VK_OEM_1;
	if (string == "PLUS")
		return VK_OEM_PLUS;
	if (string == "COMMA")
		return VK_OEM_COMMA;
	if (string == "MINUS")
		return VK_OEM_MINUS;
	if (string == "PERIOD")
		return VK_OEM_PERIOD;
	if (string == "OEM_2")
		return VK_OEM_2;
	if (string == "OEM_3")
		return VK_OEM_3;
	if (string == "OEM_4")
		return VK_OEM_4;
	if (string == "OEM_5")
		return VK_OEM_5;
	if (string == "OEM_6")
		return VK_OEM_6;
	if (string == "OEM_7")
		return VK_OEM_7;
	if (string == "OEM_8")
		return VK_OEM_8;
	if (string == "OEM_102")
		return VK_OEM_102;
	if (string == "PROCESS")
		return VK_PROCESSKEY;
	if (string == "ATTENTION")
		return VK_ATTN;
	if (string == "CRSEL")
		return VK_CRSEL;
	if (string == "EXSEL")
		return VK_EXSEL;
	if (string == "EREOF")
		return VK_EREOF;
	if (string == "PLAY")
		return VK_PLAY;
	if (string == "ZOOM")
		return VK_ZOOM;
	if (string == "PA1")
		return VK_PA1;
	if (string == "OEM_CLEAR")
		return VK_OEM_CLEAR;

	return -1;
}



/*
 * Creates a new keyboard activator from string data, used for loading from file.
 *
 * Pre: The data contains a set of keys with at least one element, and a double.
 *
 * Post: A new keyboard activator is placed onto the heap.
 */
static BaseMacroActivator* createActivator(std::string& data) {
	std::vector<int> keys;
	double reactionTime;

	std::stringstream stream(data);
	std::string token;

	// Gets activation keys.
	if (!std::getline(stream, token, ';')) {
		std::cout << "Unable to find key sequence for key activator from: " << data << std::endl;
		return nullptr;
	}

	{
		std::stringstream listStream(token);
		std::string listToken;

		while (std::getline(listStream, listToken, ',')) {
			int keyCode = getKeyFromString(listToken);

			if (keyCode != -1)
				keys.push_back(keyCode);
		}
	}

	if (!keys.size()) {
		std::cout << "Unable to find key sequence for key activator from: " << data << std::endl;
		return nullptr;
	}

	// Gets reaction time.
	if (!std::getline(stream, token, ';')) {
		std::cout << "Unable to find reaction time for key activator from: " << data << std::endl;
		return nullptr;
	}

	try {
		reactionTime = std::stod(token);

	} catch(std::invalid_argument& exception) {
		std::cout << "Unable to find reaction time for key activator from: " << data << std::endl;
		return nullptr;
	}

	std::cout << "Created a keyboard activator with " << keys.size() << " keys and a reaction time of " << reactionTime << " from the string: "
			<< data << std::endl;

	return new KeyboardActivator(keys, reactionTime);
}

/*
 * Receives key events and passes them onto keyboard activators.
 *
 * Pre: None.
 *
 * Post: Any loaded keyboard activators will receive the event, if it is a key-press.
 */
void queryActivators(KeyEvent& keyEvent) {
	if (loadedActivators.size() && (keyEvent.getFlags() & 0x8000))
		for (KeyboardActivator* activator : loadedActivators)
			activator->testKey(keyEvent.getKey());
}

KeyboardActivator::KeyboardActivator(std::vector<int> activationKeys, double reactionTime) {
	size_t vectorLength = activationKeys.size();

	if (!vectorLength)
		throw std::length_error("The length of activationKeys must be greater than 0!");

	activationLength = vectorLength;
	this->activationKeys = new int[vectorLength];
	std::copy(activationKeys.begin(), activationKeys.end(), this->activationKeys);


	this->reactionTime = reactionTime;
	timeRemaining = reactionTime;

	loadedActivators.push_back(this);
}

KeyboardActivator::KeyboardActivator(const KeyboardActivator& otherActivator) {
	int* otherKeys = otherActivator.activationKeys;

	activationLength = otherActivator.activationLength;
	activationKeys = new int[activationLength];
	std::copy(otherKeys, otherKeys + activationLength, activationKeys);


	reactionTime = otherActivator.reactionTime;
	timeRemaining = otherActivator.timeRemaining;
	currentKey = otherActivator.currentKey;
	wasExecuted = otherActivator.wasExecuted;

	loadedActivators.push_back(this);
}

KeyboardActivator::~KeyboardActivator() {
	delete[] activationKeys;
	loadedActivators.erase(std::find(loadedActivators.begin(), loadedActivators.end(), this));
}

bool KeyboardActivator::tryActivate(double deltaTime) {
	// Prevents macro from being spammed.
	if (wasExecuted) {
		bool allKeysReleased = true;

		for (size_t i = 0; i < activationLength; i++)
			if (isKeyPressed(activationKeys[i])) {
				allKeysReleased = false;
				break;
			}

		if (!allKeysReleased)
			return false;

		wasExecuted = false;
	}

	if (currentKey == activationLength) {
		currentKey = 0;
		wasExecuted = true;

		return true;

	} else if (currentKey) {
		timeRemaining -= deltaTime;

		if (timeRemaining <= 0) {
			currentKey = 0;
			timeRemaining = reactionTime;
		}
	}

	return false;
}

/*
 * Advances or resets the macro based on the incoming key.
 *
 * Pre: None.
 *
 * Post: The macro might advance or reset.
 */
void KeyboardActivator::testKey(int key) {
	if (activationKeys[currentKey] == key) {
		if (currentKey != activationLength) {
			currentKey++;
			timeRemaining = reactionTime;
		}

	// activationKeys[currentKey - 1] != key prevents multiple events of the same key from interfering.
	} else if (currentKey && activationKeys[currentKey - 1] != key){
		currentKey = 0;
		timeRemaining = reactionTime;
	}
}



KeyAction::KeyAction() {
	value = 0;
	flags = 0b01;
}

KeyAction::KeyAction(int value, int flags) {
	if ((flags & 0b11) == 0b11)
		throw std::invalid_argument("A key action cannot be both a wait and Unicode!");

	this->value = value;
	this->flags = flags;
}

int KeyAction::getValue() {
	return value;
}

bool KeyAction::isWait() {
	return flags & 0b01;
}

bool KeyAction::isUnicode() {
	return flags & 0b10;
}



/*
 * Creates a new keyboard executor from string data, used for loading from file.
 *
 * Pre: The data contains a set of keys with at least one element.
 *
 * Post: A new keyboard executor is placed onto the heap.
 */
static BaseMacroExecutor* createExecutor(std::string& data) {
	std::vector<KeyAction> keyActions;

	std::stringstream stream(data);
	std::string token;

	while (std::getline(stream, token, ',')) {
		size_t size = token.size();

		if (size > 4 && token[0] == 'W' && token[1] == 'A' && token[2] == 'I' && token[3] == 'T') {
			try {
				keyActions.push_back(KeyAction(std::stoi(token.substr(4, size)), 0b01));

			} catch(std::invalid_argument& exception) {}

		} else {
			int keyCode = getKeyFromString(token);

			if (keyCode != -1)
				keyActions.push_back(KeyAction(keyCode, 0b00));
		}
	}

	if (!keyActions.size()) {
		std::cout << "Unable to find key sequence for key executor from: " << data << std::endl;
		return nullptr;
	}

	std::cout << "Created a keyboard executor with " << keyActions.size() << " key actions from the string: " << data << std::endl;

	return new KeyboardExecutor(keyActions);
}

KeyboardExecutor::KeyboardExecutor(std::vector<KeyAction> keys) {
	size_t vectorLength = keys.size();

	if (!vectorLength)
		throw std::length_error("The length of keys must be greater than 0!");

	keysLength = vectorLength;
	this->keys = new KeyAction[vectorLength];
	std::copy(keys.begin(), keys.end(), this->keys);
}

KeyboardExecutor::KeyboardExecutor(const KeyboardExecutor& otherExecutor) {
	KeyAction* otherKeys = otherExecutor.keys;

	keysLength = otherExecutor.keysLength;
	keys = new KeyAction[keysLength];
	std::copy(otherKeys, otherKeys + keysLength, keys);
}

KeyboardExecutor::~KeyboardExecutor() {
	delete[] keys;
}

void KeyboardExecutor::execute() {
	unsigned char heldKeys = 0b000;

	for (size_t i = 0; i < keysLength; i++) {
		KeyAction& key = keys[i];
		int keyCode = key.getValue();

		if (!key.isWait()) {
			if (!key.isUnicode()) {
				switch (keyCode) {
					// The following 3 cases hold down certain keys.
					case VK_SHIFT:
						if (!(heldKeys & 0b001)) {
							heldKeys |= 0b001;
							holdVirtualKey(VK_SHIFT, false);
						}

						break;

					case VK_CONTROL:
						if (!(heldKeys & 0b010)) {
							heldKeys |= 0b010;
							holdVirtualKey(VK_CONTROL, false);
						}

						break;

					case VK_MENU:
						if (!(heldKeys & 0b100)) {
							heldKeys |= 0b100;
							holdVirtualKey(VK_CONTROL, false);
						}

						break;

					default:
						pressVirtualKey(keyCode, false);

						// Releases held keys.
						if (heldKeys) {
							if (heldKeys & 0b001)
								releaseVirtualKey(VK_SHIFT, false);

							if (heldKeys & 0b010)
								releaseVirtualKey(VK_CONTROL, false);

							if (heldKeys & 0b100)
								releaseVirtualKey(VK_MENU, false);

							heldKeys = 0;
						}
				}

			} else {
				pressVirtualKey(keyCode, true);

				// Releases held keys.
				if (heldKeys) {
					if (heldKeys & 0b001)
						releaseVirtualKey(VK_SHIFT, false);

					if (heldKeys & 0b010)
						releaseVirtualKey(VK_CONTROL, false);

					if (heldKeys & 0b100)
						releaseVirtualKey(VK_MENU, false);

					heldKeys = 0;
				}
			}

		} else
			Sleep(keyCode);
	}
}



void KeyboardMacros::initialize() {
	if (initialized)
		throw std::logic_error("KeyboardActivator is already initialized!");

	KeyboardListener::registerListener(queryActivators);
	MacroLoader::registerConstructor("Key", createActivator);
	MacroLoader::registerConstructor("Key", createExecutor);
}

void KeyboardMacros::uninitialize() {
	if (!initialized)
		throw std::logic_error("KeyboardActivator is not initialized!");

	initialized = true;
	loadedActivators.clear();
	KeyboardListener::unregisterListener(queryActivators);
	MacroLoader::unregisterConstructor("Key", false);
	MacroLoader::unregisterConstructor("Key", true);
}
