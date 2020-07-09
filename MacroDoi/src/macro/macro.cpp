//============================================================================
// Name        : macro.cpp
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Implementation of macro.h
//============================================================================

#include "macro.h"

BaseMacroActivator::~BaseMacroActivator() {}

bool BaseMacroActivator::tryActivate() {
	return false;
}



BaseMacroExecutor::~BaseMacroExecutor() {}

void BaseMacroExecutor::execute() {}



Macro::Macro(BaseMacroActivator* activator, BaseMacroExecutor* executor) {
	this->activator = activator;
	this->executor = executor;
}

Macro::Macro(const Macro& otherMacro) {;
	activator = new BaseMacroActivator(*otherMacro.activator);
	executor = new BaseMacroExecutor(*otherMacro.executor);
}

Macro::~Macro() {
	delete activator;
	delete executor;
}

void Macro::tick() {
	if (activator->tryActivate())
		executor->execute();
}
