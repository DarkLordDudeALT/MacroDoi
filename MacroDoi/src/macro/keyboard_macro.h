/**
 * Contains stuff for macros that interact with the keyboard.
 */

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
		 * @throws std::logic_error If KeyboardMacros is already initialized.
		 */
		static void initialize();

		/*
		 * Uninitializes KeyboardActivator and KeyboardExecutor.
		 *
		 * @throws std::logic_error If KeyboardMacros is not already initialized.
		 */
		static void uninitialize();
};

/*
 * A keyboard activator decides when a macro should execute based on the state of the keyboard.
 */
class KeyboardActivator : public BaseMacroActivator {
	public:
		/*
		 * Creates a new keyboard activator.
		 *
		 * @param activationKeys The key sequence needed to activate the macro. If only one key is present the activator will stutter as Windows tends to send
		 * multiple events on a single key press.
		 * @param reactionTime The amount of time the user has to press the next key before the activator resets.
		 *
		 * @throws std::length_error If activationKeys has a length less than 1.
		 */
		KeyboardActivator(std::vector<int> activationKeys, double reactionTime);

		/*
		 * Creates a copy of a keyboard activator.
		 *
		 * @param otherActivator The activator to copy.
		 */
		KeyboardActivator(const KeyboardActivator& otherActivator);

		~KeyboardActivator();

		/*
		 * Tests to see if the key-sequence has been entered, and returns the result.
		 *
		 * @param deltaTime The amount of time since the last attempt to activate.
		 *
		 * @returns Whether the key-sequence has been entered.
		 */
		bool tryActivate(double deltaTime) override;

	private:
		void testKey(int key);

		int* activationKeys;
		size_t activationLength;

		double reactionTime;
		double timeRemaining;
		size_t currentKey = 0;

	friend void queryActivators(KeyEvent& keyEvent);
};



/*
 * KeyActions represent the actions a keyboard executor should take when it executes.
 */
class KeyAction {
	public:
		/*
		 * Creates a new key action. Only here for array initialization, do not use.
		 */
		KeyAction();

		/*
		 * Creates a new key action.
		 *
		 * @param value If the first flag is set, the value represents a time, in milliseconds, to wait. If the second flag is set, it represents a Unicode
		 * character. Otherwise, it represents a Windows virtual key code.
		 * @param flags Decides what the value stands for. Flags are in big-edian order.
		 *
		 * @throws std::invalid_argument If both the first and second flags are set.
		 */
		KeyAction(int value, int flags);

		/*
		 * Gets the value of the key action.
		 *
		 * @returns The value of the key action.
		 */
		int getValue();

		/*
		 * Gets whether or not this action represents a wait action.
		 *
		 * @returns Whether or not this action represents a wait action.
		 */
		bool isWait();

		/*
		 * Gets whether or not this action represents a Unicode key.
		 *
		 * @returns Whether or not this action represents a Unicode key.
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
		 * Creates a new keyboard executor, copying from a vector of key actions that represent what it should do when activated.
		 *
		 * @param keys The key actions that represent what it should do when activated.
		 *
		 * @throws std::length_error If keys has a length less than 1.
		 */
		KeyboardExecutor(std::vector<KeyAction> keys);

		/*
		 * Creates a copy of a keyboard executor.
		 *
		 * @param otherExecutor The executor to copy.
		 */
		KeyboardExecutor(const KeyboardExecutor& otherExecutor);

		~KeyboardExecutor();

		/*
		 * Executes the functions of the macro.
		 */
		void execute() override;

	private:
		KeyAction* keys;
		size_t keysLength;
};
