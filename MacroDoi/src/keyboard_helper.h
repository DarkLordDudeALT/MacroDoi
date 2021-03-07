/**
 * Stuff to help with manipulating the keyboard.
 */

#pragma once

/*
 * Gets whether or not the given key is pressed.
 *
 * @param virtualKey The id of the virtual key.
 */
bool isKeyPressed(int virtualKey);

/*
 * Gets whether or not the given key is toggled.
 *
 * @param virtualKey The id of the virtual key.
 *
 * @returns Returns whether or not the given key is toggled.
 */
bool isKeyToggled(int virtualKey);

/*
 * "Presses" the given key.
 *
 * @param virtualKey The id of the virtual key.
 * @param isUnicode Whether the virtual key stands for a unicode character.
 */
void pressVirtualKey(int virtualKey, bool isUnicode);

/*
 * "Holds" the given key.
 *
 * @param virtualKey The id of the virtual key.
 * @param isUnicode Whether the virtual key stands for a unicode character.
 */
void holdVirtualKey(int virtualKey, bool isUnicode);

/*
 * "Releases" the given key.
 *
 * @param virtualKey The id of the virtual key.
 * @param isUnicode Whether the virtual key stands for a unicode character.
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
		 * @param virtualKey The id of the virtual key.
		 * @param flags The flags that represent the key's state.
		 * @param timesPulled The number of times the key has been updated since initial press.
		 */
		KeyEvent(int virtualKey, short flags, unsigned int timesPulled);

		/*
		 * Gets the key that was changed.
		 *
		 * @returns The key that was changed.
		 */
		int getKey();

		/*
		 * Gets the flags that represent the changed key's state.
		 *
		 * @returns The flags that represent the changed key's state.
		 */
		short getFlags();

		/*
		 * Gets the number of times the key has been updated since initial press.
		 *
		 * @returns The number of times the key has been updated since initial press.
		 */
		unsigned int getTimesPulled();

	private:
		int key;
		int flags;
		unsigned int timesPulled;
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
		 * @throws std::logic_error If KeyboardListener is already initialized.
		 */
		static void initialize();

		/*
		 * Uninitializes KeyboardListener.
		 *
		 * @throws std::logic_error If KeyboardListener is not already initialized.
		 */
		static void uninitialize();

		/*
		 * Checks for changes in the keyboard's state and notifies key listeners.
		 */
		static void tick();

		/*
		 * Registers a listener to be called when the keyboard's state changes.
		 *
		 * @param listener The listener to be called when the keyboard's state changes.
		 */
		static void registerListener(KeyListener listener);

		/*
		 * Unregisters a listener, so it will no longer be called when the keyboard's state changes.
		 *
		 * @param listener The listener to unregister.
		 */
		static void unregisterListener(KeyListener listener);
};
