/**
 * Implements keyboard_macro.h.
 */

#include "keyboard_macro.h"

#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include "../macro_loader.h"

using namespace std;

static std::vector<KeyboardActivator*> loadedActivators = std::vector<KeyboardActivator*>();
static bool initialized = false;



/**
 * djb2.
 * Computes a hash value for the given string.
 *
 * @param string The string to compute a hash value for.
 *
 * @returns The hash value for the given string
 */
static constexpr unsigned int computeHash(const char* string) {
	unsigned int hash = 5381;
	int c = 0;

	while ((c = *string++))
		hash = (hash << 5) + hash + c;

	return hash;
}

// TODO Not all keys are in this.
/*
 * Converts the code-name of a key into its key code.
 *
 * @param string The code-name of a key.
 *
 * @returns The equivalent key code, or -1, if no mapping was found.
 */
static int getKeyFromString(std::string& string) {
	switch (computeHash(string.data())) {
		case computeHash("CANCEL"):
			return VK_CANCEL;
		case computeHash("BACK"):
			return VK_BACK;
		case computeHash("TAB"):
			return VK_TAB;
		case computeHash("CLEAR"):
			return VK_CLEAR;
		case computeHash("ENTER"):
				return VK_RETURN;
		case computeHash("SHIFT"):
			return VK_SHIFT;
		case computeHash("CONTROL"):
			return VK_CONTROL;
		case computeHash("ALT"):
			return VK_MENU;
		case computeHash("PAUSE"):
			return VK_PAUSE;
		case computeHash("CAPS_LOCK"):
			return VK_CAPITAL;
		case computeHash("KANA"):
			return VK_KANA;
		case computeHash("HANGUL"):
			return VK_HANGUL;
		case computeHash("JUNJA"):
			return VK_JUNJA;
		case computeHash("FINAL"):
			return VK_FINAL;
		case computeHash("HANJA"):
			return VK_HANJA;
		case computeHash("KANJA"):
			return VK_KANJI;
		case computeHash("ESCAPE"):
			return VK_ESCAPE;
		case computeHash("CONVERT"):
			return VK_CONVERT;
		case computeHash("NONCONVERT"):
			return VK_NONCONVERT;
		case computeHash("ACCEPT"):
			return VK_ACCEPT;
		case computeHash("MODECHANGE"):
			return VK_MODECHANGE;
		case computeHash("SPACE"):
			return VK_SPACE;
		case computeHash("PAGE_UP"):
			return VK_PRIOR;
		case computeHash("PAGE_DOWN"):
			return VK_NEXT;
		case computeHash("END"):
			return VK_END;
		case computeHash("HOME"):
			return VK_HOME;
		case computeHash("LEFT_ARROW"):
			return VK_UP;
		case computeHash("UP_ARROW"):
			return VK_UP;
		case computeHash("RIGHT_ARROW"):
			return VK_RIGHT;
		case computeHash("DOWN_ARROW"):
			return VK_DOWN;
		case computeHash("SELECT"):
			return VK_SELECT;
		case computeHash("PRINT"):
			return VK_PRINT;
		case computeHash("EXECUTE"):
			return VK_EXECUTE;
		case computeHash("PRINT_SCREEN"):
			return VK_SNAPSHOT;
		case computeHash("INSERT"):
			return VK_INSERT;
		case computeHash("DELETE"):
			return VK_DELETE;
		case computeHash("HELP"):
			return VK_HELP;
		case computeHash("0"):
		case computeHash("1"):
		case computeHash("2"):
		case computeHash("3"):
		case computeHash("4"):
		case computeHash("5"):
		case computeHash("6"):
		case computeHash("7"):
		case computeHash("8"):
		case computeHash("9"):
		case computeHash("A"):
		case computeHash("B"):
		case computeHash("C"):
		case computeHash("D"):
		case computeHash("E"):
		case computeHash("F"):
		case computeHash("G"):
		case computeHash("H"):
		case computeHash("I"):
		case computeHash("J"):
		case computeHash("K"):
		case computeHash("L"):
		case computeHash("M"):
		case computeHash("N"):
		case computeHash("O"):
		case computeHash("P"):
		case computeHash("Q"):
		case computeHash("R"):
		case computeHash("S"):
		case computeHash("T"):
		case computeHash("U"):
		case computeHash("V"):
		case computeHash("W"):
		case computeHash("X"):
		case computeHash("Y"):
		case computeHash("Z"):
			return static_cast<int>(string[0]);
		case computeHash("L_WINDOWS"):
			return VK_LWIN;
		case computeHash("R_WINDOWS"):
			return VK_RWIN;
		case computeHash("APPS"):
			return VK_APPS;
		case computeHash("SLEEP"):
			return VK_SLEEP;
		case computeHash("NUMPAD_0"):
			return VK_NUMPAD0;
		case computeHash("NUMPAD_1"):
			return VK_NUMPAD1;
		case computeHash("NUMPAD_2"):
			return VK_NUMPAD2;
		case computeHash("NUMPAD_3"):
			return VK_NUMPAD3;
		case computeHash("NUMPAD_4"):
			return VK_NUMPAD4;
		case computeHash("NUMPAD_5"):
			return VK_NUMPAD5;
		case computeHash("NUMPAD_6"):
			return VK_NUMPAD6;
		case computeHash("NUMPAD_7"):
			return VK_NUMPAD7;
		case computeHash("NUMPAD_8"):
			return VK_NUMPAD8;
		case computeHash("NUMPAD_9"):
			return VK_NUMPAD9;
		case computeHash("MULTIPLY"):
			return VK_MULTIPLY;
		case computeHash("ADD"):
			return VK_ADD;
		case computeHash("SEPARATOR"):
			return VK_SEPARATOR;
		case computeHash("SUBTRACT"):
			return VK_SUBTRACT;
		case computeHash("DECIMAL"):
			return VK_DECIMAL;
		case computeHash("DIVIDE"):
			return VK_DIVIDE;
		case computeHash("F1"):
			return VK_F1;
		case computeHash("F2"):
			return VK_F2;
		case computeHash("F3"):
			return VK_F3;
		case computeHash("F4"):
			return VK_F4;
		case computeHash("F5"):
			return VK_F5;
		case computeHash("F6"):
			return VK_F6;
		case computeHash("F7"):
			return VK_F7;
		case computeHash("F8"):
			return VK_F8;
		case computeHash("F9"):
			return VK_F9;
		case computeHash("F10"):
			return VK_F10;
		case computeHash("F11"):
			return VK_F11;
		case computeHash("F12"):
			return VK_F12;
		case computeHash("F13"):
			return VK_F13;
		case computeHash("F14"):
			return VK_F14;
		case computeHash("F15"):
			return VK_F15;
		case computeHash("F16"):
			return VK_F16;
		case computeHash("F17"):
			return VK_F17;
		case computeHash("F18"):
			return VK_F18;
		case computeHash("F19"):
			return VK_F19;
		case computeHash("F20"):
			return VK_F20;
		case computeHash("F21"):
			return VK_F21;
		case computeHash("F22"):
			return VK_F22;
		case computeHash("F23"):
			return VK_F23;
		case computeHash("F24"):
			return VK_F24;
		case computeHash("NUMBER_LOCK"):
			return VK_NUMLOCK;
		case computeHash("SCROLL"):
			return VK_SCROLL;
		case computeHash("LEFT_SHIFT"):
			return VK_LSHIFT;
		case computeHash("RIGHT_SHIFT"):
			return VK_RSHIFT;
		case computeHash("LEFT_CONTROL"):
			return VK_LCONTROL;
		case computeHash("RIGHT_CONTROL"):
			return VK_RCONTROL;
		case computeHash("LEFT_ALT"):
			return VK_LMENU;
		case computeHash("RIGHT_ALT"):
			return VK_RMENU;
		case computeHash("BROWSER_BACK"):
			return VK_BROWSER_BACK;
		case computeHash("BROWSER_FORWARD"):
			return VK_BROWSER_FORWARD;
		case computeHash("BROWSER_REFRESH"):
			return VK_BROWSER_REFRESH;
		case computeHash("BROWSER_STOP"):
			return VK_BROWSER_STOP;
		case computeHash("BROWSER_SEARCH"):
			return VK_BROWSER_SEARCH;
		case computeHash("BROWSER_FAVORITES"):
			return VK_BROWSER_FAVORITES;
		case computeHash("BROWSER_HOME"):
			return VK_BROWSER_HOME;
		case computeHash("VOLUME_MUTE"):
			return VK_VOLUME_MUTE;
		case computeHash("VOLUME_DOWN"):
			return VK_VOLUME_DOWN;
		case computeHash("VOLUME_UP"):
			return VK_VOLUME_UP;
		case computeHash("NEXT_TRACK"):
			return VK_MEDIA_NEXT_TRACK;
		case computeHash("PREVIOUS_TRACK"):
			return VK_MEDIA_PREV_TRACK;
		case computeHash("MEDIA_STOP"):
			return VK_MEDIA_STOP;
		case computeHash("PLAY_PAUSE"):
			return VK_MEDIA_PLAY_PAUSE;
		case computeHash("LAUNCH_MAIL"):
			return VK_LAUNCH_MAIL;
		case computeHash("MEDIA_SELECT"):
			return VK_LAUNCH_MEDIA_SELECT;
		case computeHash("LAUNCH_APP1"):
			return VK_LAUNCH_APP1;
		case computeHash("LAUNCH_APP2"):
			return VK_LAUNCH_APP2;
		case computeHash("OEM_1"):
			return VK_OEM_1;
		case computeHash("PLUS"):
			return VK_OEM_PLUS;
		case computeHash("COMMA"):
			return VK_OEM_COMMA;
		case computeHash("MINUS"):
			return VK_OEM_MINUS;
		case computeHash("PERIOD"):
			return VK_OEM_PERIOD;
		case computeHash("OEM_2"):
			return VK_OEM_2;
		case computeHash("OEM_3"):
			return VK_OEM_3;
		case computeHash("OEM_4"):
			return VK_OEM_4;
		case computeHash("OEM_5"):
			return VK_OEM_5;
		case computeHash("OEM_6"):
			return VK_OEM_6;
		case computeHash("OEM_7"):
			return VK_OEM_7;
		case computeHash("OEM_8"):
			return VK_OEM_8;
		case computeHash("OEM_102"):
			return VK_OEM_102;
		case computeHash("PROCESS"):
			return VK_PROCESSKEY;
		case computeHash("ATTENTION"):
			return VK_ATTN;
		case computeHash("CRSEL"):
			return VK_CRSEL;
		case computeHash("EXSEL"):
			return VK_EXSEL;
		case computeHash("EREOF"):
			return VK_EREOF;
		case computeHash("PLAY"):
			return VK_PLAY;
		case computeHash("ZOOM"):
			return VK_ZOOM;
		case computeHash("PA1"):
			return VK_PA1;
		case computeHash("OEM_CLEAR"):
			return VK_OEM_CLEAR;
	}

	return -1;
}



/*
 * Creates a new keyboard activator from string data, used for loading from file.
 *
 * @param data The data to create a keyboard activator from.
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
		std::cout << "Error: There must be at least one valid key. Rejected: " << data << std::endl;
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

	if (reactionTime <= 0) {
		std::cout << "Error: Reaction time must be greater than zero. Rejected: " << data << std::endl;
		return nullptr;
	}

	std::cout << "Created a keyboard activator with " << keys.size() << " keys and a reaction time of " << reactionTime << " from the string: "
			<< data << std::endl;

	return new KeyboardActivator(keys, reactionTime);
}

/*
 * Receives key events and passes them onto keyboard activators.
 *
 * @param keyEvent The key event to pass onto the keyboard activators.
 */
void queryActivators(KeyEvent& keyEvent) {
	if (loadedActivators.size() && (keyEvent.getFlags() & 0x8000) && keyEvent.getTimesPulled() == 1)
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

	loadedActivators.push_back(this);
}

KeyboardActivator::~KeyboardActivator() {
	delete[] activationKeys;
	loadedActivators.erase(std::find(loadedActivators.begin(), loadedActivators.end(), this));
}

bool KeyboardActivator::tryActivate(double deltaTime) {
	if (currentKey == activationLength) {
		currentKey = 0;

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
 * @param key The key that was pressed.
 */
void KeyboardActivator::testKey(int key) {
	if (activationKeys[currentKey] == key && currentKey != activationLength) {
		currentKey++;
		timeRemaining = reactionTime;

	} else if (currentKey) {
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
 * @param data The data to create a keyboard executor from.
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
		std::cout << "Error: There must be at least one valid key. Rejected: " << data << std::endl;
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
	std::thread executionThread([](KeyboardExecutor* executor) {
		unsigned char heldKeys = 0b000;

		for (size_t i = 0; i < executor->keysLength; i++) {
			KeyAction& key = executor->keys[i];
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
				this_thread::sleep_for(chrono::milliseconds(keyCode));
		}
	}, this);

	executionThread.detach();
}



void KeyboardMacros::initialize() {
	if (initialized)
		throw std::logic_error("KeyboardActivator is already initialized!");

	KeyboardListener::registerListener(queryActivators);
	MacroLoader::registerConstructor("Key", createActivator);
	MacroLoader::registerConstructor("Key", createExecutor); // @suppress("Ambiguous problem")
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
