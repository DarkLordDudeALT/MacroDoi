//============================================================================
// Name        : macro_loader.cpp
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Implementation of macro_loader.h
//============================================================================

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

// TODO Add comments to macro loading.
void MacroLoader::loadMacrosFromFile() {
	if (loaded)
		throw std::logic_error("Macros are already loaded!");

	std::ifstream macroFile;
	macroFile.open("macros.txt");

	if (!macroFile.is_open()) {
		std::cout << "Could not open file 'macros.txt', make sure to create it" << std::endl << "exiting program" << std::endl;
		exit(EXIT_FAILURE);
	}

	for (std::string line; std::getline(macroFile, line);) {
		BaseMacroActivator* activator;

		for (auto lineIterator = line.begin(); lineIterator < line.end(); lineIterator++)
			if (*lineIterator == ' ' || *lineIterator == '\n' || *lineIterator == '\r')
				line.erase(lineIterator);

		if (line == "" || line[0] == '#')
			continue;

		bool fail = false;
		std::stringstream lineStream(line);
		std::string token;

		// Gets activator.
		if (std::getline(lineStream, token, ':')) {
			std::stringstream activatorStream(token);
			std::string secondaryToken;

			if (std::getline(activatorStream, secondaryToken, '|')) {
				std::string activatorName(secondaryToken);

				if (activatorConstructors.find(activatorName) != activatorConstructors.end() && std::getline(activatorStream, secondaryToken, '|')) {
					activator = activatorConstructors.at(activatorName)(secondaryToken);

				} else
					fail = true;

			} else
				fail = true;

		} else
			fail = true;

		BaseMacroExecutor* executor;

		if (fail || !activator) {
			std::cout << "Unable to find activator from: " << line << std::endl;
			continue;
		}

		// Gets executor.
		if (std::getline(lineStream, token, ':')) {
			std::stringstream executorStream(token);
			std::string secondaryToken;

			if (std::getline(executorStream, secondaryToken, '|')) {
				std::string executorName(secondaryToken);

				if (executorConstructors.find(executorName) != executorConstructors.end() && std::getline(executorStream, secondaryToken, '|')) {
					executor = executorConstructors.at(executorName)(secondaryToken);

				} else
					fail = true;

			} else
				fail = true;

		} else
			fail = true;

		if (fail || !executor) {
			std::cout << "Unable to find executor from: " << line << std::endl;
			delete activator;

		} else
			loadedMacros.push_back(new Macro(activator, executor));
	}

	macroFile.close();
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
