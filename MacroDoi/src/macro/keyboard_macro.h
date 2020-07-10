//============================================================================
// Name        : keyboard_macro.h
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Contains stuff for macros that interact with the keyboard
//============================================================================

#pragma once

#include "macro.h"

#include <vector>
#include "../keyboard_helper.h"

class KeyboardActivator : public BaseMacroActivator {
	public:
		static void initialize();

		static void uninitialize();


		KeyboardActivator(std::vector<int> activationKeys, double reactionTime);

		KeyboardActivator(const KeyboardActivator& otherActivator);

		~KeyboardActivator();

		bool tryActivate(double deltaTime) override;

	private:
		void testKey(int key);

		int* activationKeys;
		size_t activationLength;

		double reactionTime;
		double timeRemaining;
		size_t currentKey = 0;
		bool wasExecuted = false;

	friend void queryActivators(KeyEvent& keyEvent);
};



class KeyAction {
	public:
		KeyAction();

		KeyAction(int value, bool isWait);

		int getValue();

		bool isWait();

	private:
		int value;
		bool wait;
};


class KeyboardExecutor : public BaseMacroExecutor {
	public:
		KeyboardExecutor(std::vector<KeyAction> keys);

		KeyboardExecutor(const KeyboardExecutor& otherExecutor);

		~KeyboardExecutor();

		void execute() override;

	private:
		KeyAction* keys;
		size_t keysLength;
};
