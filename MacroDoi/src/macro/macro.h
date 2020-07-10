//============================================================================
// Name        : macro.h
// Author      : Epiphany
// Version     : null
// Copyright   : I don't care lol
// Description : Macros and their components
//============================================================================

#pragma once

/*
 * The base class for macro activators.
 * Macro activators decide when a macro should be activated.
 */
class BaseMacroActivator {
	public:
		virtual ~BaseMacroActivator();

		/*
		 * Decides whether or not a macro should activate, and returns the result.
		 *
		 * Pre: Depends on extending classes. deltaTime must be greater than zero.
		 *
		 * Post: Whether or not the macro should activate, and returns the result.
		 */
		virtual bool tryActivate(double deltaTime);
};


/*
 * The base class for macro executors.
 * Macro executors decide what happens when a macro is activated.
 */
class BaseMacroExecutor {
	public:
		virtual ~BaseMacroExecutor();

		/*
		 * Executes the functions of the executor.
		 * Called when it's conjugate activator says it's go-time.
		 *
		 * Pre: None.
		 *
		 * Post: This executor executes.
		 */
		virtual void execute();
};



/*
 * A macro, the combination of an activator and an executor.
 */
class Macro {
	public:
		/*
		 * Creates a new macro.
		 * The given activator and executor are entangled with it. Changes made to them will affect the macro.
		 *
		 * Pre: None.
		 *
		 * Post: A macro is created, and the given activator and executor are entangled with it.
		 */
		Macro(BaseMacroActivator* activator, BaseMacroExecutor* executor);

		/*
		 * Creates a copy of the given macro.
		 * Will create new instances of the activator and executor.
		 * Changes made to one macro will not affect the other.
		 *
		 * Pre: None.
		 *
		 * Post: A new macro is created.
		 */
		Macro(const Macro& otherMacro);

		/*
		 * Destroys a macro, along with its activator and executor.
		 *
		 * Pre: None.
		 *
		 * Post: The macro, its activator, and its executor are destroyed.
		 */
		~Macro();

		/*
		 * Attempts to execute the macro by querying the activator.
		 *
		 * Pre: deltaTime must be greater than 0.
		 *
		 * Post: The macro might execute.
		 */
		void tick(double deltaTime);

	private:
		BaseMacroActivator* activator;
		BaseMacroExecutor* executor;
};
