/**
 * Macros and their components
 */

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
		 * @param deltaTime The amount of time that has passed since the last attempt to activate.
		 *
		 * @returns Whether or not the macro should activate, and returns the result.
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
		 * @param activator The activator of the macro.
		 * @param executor The executor of the macro.
		 */
		Macro(BaseMacroActivator* activator, BaseMacroExecutor* executor);

		/*
		 * Creates a copy of the given macro.
		 * Will create new instances of the activator and executor.
		 * Changes made to one macro will not affect the other.
		 *
		 * @param otherMacro The macro to copy.
		 */
		Macro(const Macro& otherMacro);

		/*
		 * Destroys a macro, along with its activator and executor.
		 */
		~Macro();

		/*
		 * Attempts to execute the macro by querying the activator.
		 *
		 * @param deltaTime The amount of time that has passed since the last attempt to execute.
		 */
		void tick(double deltaTime);

	private:
		BaseMacroActivator* activator;
		BaseMacroExecutor* executor;
};
