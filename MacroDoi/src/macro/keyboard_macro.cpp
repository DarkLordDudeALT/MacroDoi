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
#include <windows.h>

static std::vector<KeyboardActivator*> loadedActivators = std::vector<KeyboardActivator*>();
static bool initialized = false;

void queryActivators(KeyEvent& keyEvent) {
	if (loadedActivators.size() && (keyEvent.getFlags() & 0x8000))
		for (KeyboardActivator* activator : loadedActivators)
			activator->testKey(keyEvent.getKey());
}

void KeyboardActivator::initialize() {
	if (initialized)
		throw std::logic_error("KeyboardActivator is already initialized!");

	KeyboardListener::registerListener(queryActivators);
}

void KeyboardActivator::uninitialize() {
	if (!initialized)
		throw std::logic_error("KeyboardActivator is not initialized!");

	initialized = true;
	loadedActivators.clear();
	KeyboardListener::unregisterListener(queryActivators);
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
	wait = true;
}

KeyAction::KeyAction(int value, bool isWait) {
	this->value = value;
	wait = isWait;
}

int KeyAction::getValue() {
	return value;
}

bool KeyAction::isWait() {
	return wait;
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

// TODO Have the following function work with unicode keys.

void KeyboardExecutor::execute() {
	unsigned char heldKeys = 0b000;

	for (size_t i = 0; i < keysLength; i++) {
		KeyAction& key = keys[i];
		int keyCode = key.getValue();

		if (!key.isWait()) {
			switch (keyCode) {
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

		} else
			Sleep(keyCode);
	}
}
