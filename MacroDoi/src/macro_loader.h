#pragma once

#include "macro/macro.h"
#include <string>

typedef BaseMacroActivator*(*ActivatorConstructor)(std::string&);
typedef BaseMacroExecutor*(*ExecutorConstructor)(std::string&);

class MacroLoader {
	public:
		static void loadMacrosFromFile();

		static void unloadMacros();

		static void registerConstructor(std::string key, ActivatorConstructor constructor);

		static void registerConstructor(std::string key, ExecutorConstructor constructor);

		static void unregisterConstructor(std::string key, bool isExecutor);

		static void tick(double deltaTime);
};
