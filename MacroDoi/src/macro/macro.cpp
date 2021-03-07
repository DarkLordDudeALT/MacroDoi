/**
 * Implementation of macro.h.
 */

#include "macro.h"

BaseMacroActivator::~BaseMacroActivator() {}

bool BaseMacroActivator::tryActivate(double deltaTime) {
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

void Macro::tick(double deltaTime) {
	if (activator->tryActivate(deltaTime))
		executor->execute();
}
