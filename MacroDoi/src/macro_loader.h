//============================================================================
// Name        : macro_loader.cpp
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Loads macros from file, allowing easy user-level configuration
//============================================================================

#pragma once

#include "macro/macro.h"
#include <string>

typedef BaseMacroActivator*(*ActivatorConstructor)(std::string&);
typedef BaseMacroExecutor*(*ExecutorConstructor)(std::string&);

/*
 * Code for loading macros from files.
 * Constructors should be registered before loading.
 */
class MacroLoader {
	public:
		/*
		 * Loads macros from file.
		 *
		 * Pre: Macros are not already loaded. Constructors are registered. macros.txt exists.
		 *
		 * Post: Any valid macros are loaded from macros.txt.
		 */
		static void loadMacrosFromFile();

		/*
		 * Unloads macros and unregisters constructors.
		 *
		 * Pre: Macros are loaded.
		 *
		 * Post: Macros are no longer loaded.
		 */
		static void unloadMacros();

		/*
		 * Registers an activator constructor under the given key.
		 * The key is what users specify in-file to pick an activator.
		 *
		 * Pre: The constructor is not already registered.
		 *
		 * Post: The constructor will now be called if a user specifies for it.
		 */
		static void registerConstructor(std::string key, ActivatorConstructor constructor);

		/*
		 * Registers an executor constructor under the given key.
		 * The key is what users specify in-file to pick an executor.
		 *
		 * Pre: The constructor is not already registered.
		 *
		 * Post: The constructor will now be called if a user specifies for it.
		 */
		static void registerConstructor(std::string key, ExecutorConstructor constructor);

		/*
		 * Unregisters a constructor under the given key.
		 * The key is what users specify in-file to pick an activator or executor.
		 *
		 * Pre: The constructor is registered.
		 *
		 * Post: The constructor is no longer registered.
		 */
		static void unregisterConstructor(std::string key, bool isExecutor);

		/*
		 * Steps forward all loaded macros.
		 *
		 * Pre: None.
		 *
		 * Post: All loaded macros are ticked.
		 */
		static void tick(double deltaTime);
};
