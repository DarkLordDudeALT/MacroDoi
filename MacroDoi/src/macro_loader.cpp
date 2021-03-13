/**
 * Implementation of macro_loader.h.
 */

#include "macro_loader.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <stringapiset.h>

static bool loaded = false;
static std::vector<Macro*> loadedMacros = std::vector<Macro*>();

static std::unordered_map<std::string, ActivatorConstructor> activatorConstructors = std::unordered_map<std::string, ActivatorConstructor>();
static std::unordered_map<std::string, ExecutorConstructor> executorConstructors = std::unordered_map<std::string, ExecutorConstructor>();

void MacroLoader::loadMacrosFromFile() {
	if (loaded)
		throw std::logic_error("Macros are already loaded!");

	std::ifstream macroFile;
	macroFile.open("macros.txt");

	if (!macroFile.is_open()) {
		std::cout << "ERROR: Could not open file 'macros.txt':" << std::endl;
		std::cerr << "\t" << strerror(errno) << std::endl << std::endl;

		std::cout << "Press ENTER or RETURN to exit the program.";
		std::cin.get();
		exit(EXIT_FAILURE);
	}

	unsigned long int currentLine = 0;
	for (std::string line; std::getline(macroFile, line); currentLine++) {
		BaseMacroActivator* activator;

		// Removes comments starting from the delimiter to the end of the string.
		size_t findResult = line.find('#');
		if (findResult != (size_t) -1)
			line.replace(findResult, line.length() - findResult, "");

		// Removes whitespace characters.
		for (auto lineIterator = line.begin(); lineIterator < line.end(); lineIterator++)
			if (*lineIterator == ' ' || *lineIterator == '\n' || *lineIterator == '\r' || *lineIterator == '\t')
				line.erase(lineIterator);

		if (line == "")
			continue;


		std::stringstream lineStream(line);
		std::string token;
		bool fail = false;

		// Gets activator.
		if (std::getline(lineStream, token, '|') && lineStream.str().length() != token.length()) {
			std::stringstream activatorStream(token);
			std::string secondaryToken;

			// Gets activator name.
			if (std::getline(activatorStream, secondaryToken, ':') && activatorStream.str().length() != secondaryToken.length()) {
				std::string activatorName(secondaryToken);

				// Finds correct constructor, gets activator parameters, and creates a new activator.
				if (activatorConstructors.find(activatorName) != activatorConstructors.end()) {
					std::getline(activatorStream, secondaryToken, '|');
					activator = activatorConstructors.at(activatorName)(secondaryToken);

				} else {
					std::cout << "ERROR: " << activatorName << " is not a known activator type." << std::endl;
					fail = true;
				}

			} else {
				std::cout << "ERROR: Unable to find activator name/elements separator ':'." << std::endl;
				fail = true;
			}

		} else {
			std::cout << "ERROR: Unable to find activator/executor separator '|'. Must be located between activator and separator deceleration." << std::endl;
			fail = true;
		}

		BaseMacroExecutor* executor;

		if (fail || !activator) {
			std::cout << "Unable to create activator from line " << currentLine << " (" << line << ")." << std::endl << std::endl;
			continue;
		}

		// Gets executor.
		if (std::getline(lineStream, token, '|')) {
			std::stringstream executorStream(token);
			std::string secondaryToken;

			// Gets executor name.
			if (std::getline(executorStream, secondaryToken, ':') && executorStream.str().length() != secondaryToken.length()) {
				std::string executorName(secondaryToken);

				// Finds correct constructor, gets executor parameters, and creates a new executor.
				if (executorConstructors.find(executorName) != executorConstructors.end()) {
					std::getline(executorStream, secondaryToken, '|');
					executor = executorConstructors.at(executorName)(secondaryToken);

				} else {
					std::cout << "ERROR: " << executorName << " is not a known executor type." << std::endl;
					fail = true;
				}

			} else {
				std::cout << "ERROR: Unable to find executor name/elements separator ':'." << std::endl;
				fail = true;
			}
		}

		if (fail || !executor) {
			std::cout << "Unable to create executor from line " << currentLine << " (" << line << ")." << std::endl << std::endl;
			delete activator;

		} else
			loadedMacros.push_back(new Macro(activator, executor));
	}

	macroFile.close();

	if (!loadedMacros.size()) {
		std::cout << "Unable to load any macros." << std::endl;

		std::cout << "Press ENTER or RETURN to exit the program.";
		std::cin.get();
		exit(-1);
	}

	std::cout << loadedMacros.size() << " macros loaded." << std::endl;

	loaded = true;
}

void unloadMacros() {
	if (!loaded)
		throw std::logic_error("Macros are not loaded!");

	for (Macro* macro : loadedMacros)
		delete[] macro;

	loadedMacros.clear();
	loaded = false;
}

void MacroLoader::registerConstructor(std::string key, ActivatorConstructor constructor) {
	auto result = activatorConstructors.find(key);

	if (result == activatorConstructors.end())
		activatorConstructors.insert({key, constructor});
}

void MacroLoader::registerConstructor(std::string key, ExecutorConstructor constructor) {
	auto result = executorConstructors.find(key);

	if (result == executorConstructors.end())
		executorConstructors.insert({key, constructor});
}

void MacroLoader::unregisterConstructor(std::string key, bool isExecutor) {
	if (!isExecutor) {
		auto result = activatorConstructors.find(key);

		if (result != activatorConstructors.end())
			activatorConstructors.erase(result);

	} else {
		auto result = executorConstructors.find(key);

		if (result != executorConstructors.end())
			executorConstructors.erase(result);
	}
}

void MacroLoader::tick(double deltaTime) {
	for (Macro* macro : loadedMacros)
		macro->tick(deltaTime);
}
