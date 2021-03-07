/**
 * Loads macros from file, allowing easy user-level configuration.
 */

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
		 * Exits and alerts the user if the file cannot be found.
		 *
		 * @throws std::logic_error If the macros are already loaded.
		 */
		static void loadMacrosFromFile();

		/*
		 * Unloads macros and unregisters constructors.
		 *
		 * @throws std::logic_error If the macros are not already loaded.
		 */
		static void unloadMacros();

		/*
		 * Registers an activator constructor under the given key.
		 *
		 * @param key Name of the activator. Users use this to specify which activator to use.
		 * @param constructor The constructor method for the activator.
		 */
		static void registerConstructor(std::string key, ActivatorConstructor constructor);

		/*
		 * Registers an executor constructor under the given key.
		 *
		 * @param key Name of the executor. Users use this to specify which executor to use.
		 * @param constructor The constructor method for the executor.
		 */
		static void registerConstructor(std::string key, ExecutorConstructor constructor);

		/*
		 * Unregisters a constructor under the given key.
		 *
		 * @param key Name of the activator/executor. Users use this to specify which activator/executor to use.
		 * @param isExecutor Whether the constructor is for an executor or an activator.
		 */
		static void unregisterConstructor(std::string key, bool isExecutor);

		/*
		 * Steps forward all loaded macros.
		 *
		 * @param deltaTime The amount of time that has passed since the last tick.
		 */
		static void tick(double deltaTime);
};
