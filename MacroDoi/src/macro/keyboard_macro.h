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
#include <string>
#include "../keyboard_helper.h"

/*
 * Code to enable keyboard macros.
 */
class KeyboardMacros {
	public:
		/*
		 * Initializes KeyboardActivator and KeyboardExecutor.
		 *
		 * Pre: They are not already initialized. They must be initialized beforehand.
		 *
		 * Post: They are initialized.
		 */
		static void initialize();

		/*
		 * Uninitializes KeyboardActivator and KeyboardExecutor.
		 *
		 * Pre: They are already initialized. They must not be uninitialized beforehand.
		 *
		 * Post: They are uninitialized.
		 */
		static void uninitialize();
};

/*
 * A keyboard activator decides when a macro should execute based on the state of the keyboard.
 */
class KeyboardActivator : public BaseMacroActivator {
	public:
		/*
		 * Creates a new keyboard activator. The key-sequence is copied from activationKeys.
		 * reactionTime is the amount of time the user has to press the next key, or else it will reset.
		 *
		 * Pre: activationKeys must have at least one element. Note: having only one element will lead to the macro repeating itself,
		 * as Windows sends multiple messages for held keys.
		 *
		 * Post: A new keyboard activator is created.
		 */
		KeyboardActivator(std::vector<int> activationKeys, double reactionTime);

		/*
		 * Creates a copy of a keyboard activator.
		 *
		 * Pre: None.
		 *
		 * Post: A new keyboard activator is created.
		 */
		KeyboardActivator(const KeyboardActivator& otherActivator);

		/*
		 * Destroys a keyboard activator.
		 *
		 * Pre: None.
		 *
		 * Post: A keyboard activator is destroyed.
		 */
		~KeyboardActivator();

		/*
		 * Tests to see if the key-sequence has been entered, and returns the result.
		 * deltaTime represents the time since last execution.
		 *
		 * Pre: None.
		 *
		 * Post: The result is returned.
		 */
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



/*
 * KeyActions represent the actions a keyboard executor should take when it executes.
 */
class KeyAction {
	public:
		/*
		 * Creates a new key action. Only here for arrays.
		 *
		 * Pre: None.
		 *
		 * Post: A new key action is created..
		 */
		KeyAction();

		/*
		 * Creates a new key action. Flags are in big-edian order.
		 * If the first flag is set, the value represents a time, in milliseconds, to wait.
		 * If the second flag is set, the value represents a Unicode character.
		 * Else, the value represents a Windows virtual key.
		 *
		 * Pre: None.
		 *
		 * Post: A new key action is created.
		 */
		KeyAction(int value, int flags);

		/*
		 * Gets the value of the key action.
		 *
		 * Pre: None.
		 *
		 * Post: Returns the value of the key action.
		 */
		int getValue();

		/*
		 * Gets whether or not this action represents a wait action.
		 *
		 * Pre: None.
		 *
		 * Post: Returns whether or not this action represents a wait action.
		 */
		bool isWait();

		/*
		 * Gets whether or not this action represents a Unicode key.
		 *
		 * Pre: None.
		 *
		 * Post: Returns whether or not this action represents a Unicode key.
		 */
		bool isUnicode();

	private:
		int value;
		int flags;
};


/*
 * A keyboard Executor executes actions that involve the keyboard when a macro is activated.
 */
class KeyboardExecutor : public BaseMacroExecutor {
	public:
		/*
		 * Creates a new keyboard executor, copying from a vector of key actions that represent what it should do.
		 *
		 * Pre: keys must have at least one element.
		 *
		 * Post: A new keyboard executor is created.
		 */
		KeyboardExecutor(std::vector<KeyAction> keys);

		/*
		 * Creates a copy of a keyboard executor.
		 *
		 * Pre: None.
		 *
		 * Post: A new keyboard executor is created.
		 */
		KeyboardExecutor(const KeyboardExecutor& otherExecutor);

		/*
		 * Destroys a keyboard executor.
		 *
		 * Pre: None.
		 *
		 * Post: A keyboard executor is destroyed.
		 */
		~KeyboardExecutor();

		/*
		 * Executes the functions of this macro.
		 * Those functions include synthesizing key presses, and waiting.
		 *
		 * Pre: None.
		 *
		 * Post: The functions of this macro are executed.
		 */
		void execute() override;

	private:
		KeyAction* keys;
		size_t keysLength;
};
