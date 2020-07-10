//============================================================================
// Name        : keyboard_helper.h
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Stuff to help with manipulating the keyboard
//============================================================================

#pragma once

/*
 * Gets whether or not the given key is pressed.
 *
 * Pre: None.
 *
 * Post: Returns whether or not the given key is pressed.
 */
bool isKeyPressed(int virtualKey);

/*
 * Gets whether or not the given key is toggled.
 *
 * Pre: None.
 *
 * Post: Returns whether or not the given key is toggled.
 */
bool isKeyToggled(int virtualKey);

/*
 * "Presses" the given key.
 *
 * Pre: If the given key is a unicode value, isUnicode must be true.
 * Else, it must be false.
 *
 * Post: The given key is "pressed."
 */
void pressVirtualKey(int virtualKey, bool isUnicode);

/*
 * "Holds" the given key.
 *
 * Pre: If the given key is a unicode value, isUnicode must be true.
 * Else, it must be false.
 *
 * Post: The given key is "held."
 */
void holdVirtualKey(int virtualKey, bool isUnicode);

/*
 * "Releases" the given key.
 *
 * Pre: If the given key is a unicode value, isUnicode must be true.
 * Else, it must be false.
 *
 * Post: The given key is "released."
 */
void releaseVirtualKey(int virtualKey, bool isUnicode);



/*
 * Represents a key press.
 * Created when the keyboard's state changes.
 */
class KeyEvent {
	public:
		/*
		 * Creates a new key event.
		 *
		 * Pre: None.
		 *
		 * Post: A new key event is created.
		 */
		KeyEvent(int virtualKey, short flags);

		/*
		 * Gets the key that was changed.
		 *
		 * Pre: None.
		 *
		 * Post: Returns the key that was changed.
		 */
		int getKey();

		/*
		 * Gets the flags that represent the changed key's state.
		 *
		 * Pre: None.
		 *
		 * Post: Returns the flags that represent the changed key's state.
		 */
		short getFlags();

	private:
		int key;
		int flags;
};


typedef void(*KeyListener)(KeyEvent&);

/*
 * Listens for changes in the keyboard's state and notifies key listeners of them.
 */
class KeyboardListener {
	public:
		/*
		 * Initializes KeyboardListener.
		 *
		 * Pre: KeyboardListener is not already initialized.
		 *
		 * Post: KeyboardListener is initialized.
		 */
		static void initialize();

		/*
		 * Uninitializes KeyboardListener.
		 *
		 * Pre: KeyboardListener is initialized.
		 *
		 * Post: KeyboardListener is uninitialized.
		 */
		static void uninitialize();

		/*
		 * Checks for changes in the keyboard's state and notifies key listeners.
		 *
		 * Pre: KeyboardListener is initialized.
		 *
		 * Post: The key listeners are notified if anything changes.
		 */
		static void tick();

		/*
		 * Registers a listener to be called when the keyboard's state changes.
		 *
		 * Pre: The listener is not already registered
		 *
		 * Post: The listener will now be called when the keyboard's state changes.
		 */
		static void registerListener(KeyListener listener);

		/*
		 * Unregisters a listener, so it will no longer be called when the keyboard's state changes.
		 *
		 * Pre: The listener is registered.
		 *
		 * Post: The listener will no longer be called when the keyboard's state changes.
		 */
		static void unregisterListener(KeyListener listener);
};
