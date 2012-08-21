/*
 * GameLoop.h
 *
 *  Created on: 13.03.2012
 *      Author: yoshi252
 */

#ifndef GAMELOOP_H_
#define GAMELOOP_H_

#include <functional>
#include <vector>
#include <cstdint>
namespace TaskReturnvalue {
enum TaskReturnvalue {
	OK,
	EXIT
};
}

/*! \brief The main loop of a game.
 *
 *  It allows to add multiple callbacks that will be called at a given
 *  frequencies.
 *  This class also allows to set a callback to draw the screen as often
 *  as possible.
 *  The class uses SDL as a timing source, so you should initialize
 *  SDL before calling the run() method.
 */
class GameLoop {
public:
	GameLoop();
	virtual ~GameLoop();

	/*! \brief Set the callback for drawing the screen.
	 *
	 *  If a callback for the draw task was already set the callback will
	 *  be replaced.
	 *  \param task a callback to be used to draw the screen
	 *  \return none
	 */
	void setDrawTask(std::function<void(void)> task);

	/*! \brief Add a task that is to be executed at a given frequency.
	 *
	 *  \param task a callback that is to be called at a given frequency
	 *  \param frequency the frequency the callback is to be executed in Hz
	 *  \return none
	 */
	void addCycleTask(std::function<TaskReturnvalue::TaskReturnvalue(void)> task, float frequency);

	/*! \brief Run the mainloop.
	 *
	 *  This loop will run as long as the cycle tasks do not return
	 *  TaskReturnvalue::EXIT.
	 *
	 *  \warning If you did not set a drawing callback using setDrawTask()
	 *           a std::bad_function_call exception will be raised.
	 */
	void run();

private:

	struct CycleTask {
		uint64_t cycleTime;   // in us
		uint64_t elapsedTime; // in us

		std::function<TaskReturnvalue::TaskReturnvalue(void)> task;
	};

	std::vector<CycleTask> timedTasks;  //!< the tasks to be called at a given frequency
	std::function<void(void)> drawTask; //!< the task to draw the screen

};

#endif /* GAMELOOP_H_ */
