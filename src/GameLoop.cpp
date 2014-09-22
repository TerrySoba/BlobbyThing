/*
 * GameLoop.cpp
 *
 *  Created on: 13.03.2012
 *      Author: yoshi252
 */

#include "SDL.h"
#include "GameLoop.h"

#include "ErrorLogging.h"

GameLoop::GameLoop() {
	// TODO Auto-generated constructor stub

}

GameLoop::~GameLoop() {
	// TODO Auto-generated destructor stub
}

void GameLoop::setDrawTask(std::function<void(void)> task) {
	drawTask = task;
}

void GameLoop::addCycleTask(std::function<TaskReturnvalue::TaskReturnvalue(void)> task, float frequency) {
	CycleTask t;
	t.elapsedTime = 0;
	t.cycleTime = 1000000.0 / frequency;
	if (t.cycleTime <= 0) {
		LOG("Frequency of ", frequency, " Hz results in cycle time < 1us. Setting frequency to 1MHz.");
		t.cycleTime = 1;
	}
	t.task = task;
	this->timedTasks.push_back(t);
}

void GameLoop::run() {
	// at first initialize all tasks with current time
	uint64_t currentTime = (uint64_t)SDL_GetTicks() * (uint64_t)1000;
	for(CycleTask &task: this->timedTasks) {
		task.elapsedTime = currentTime;
	}

	bool exit = false;

	// now enter mainloop
	do {
		currentTime = (uint64_t)SDL_GetTicks() * (uint64_t)1000;

		// execute CycleTasks first and in interleaved order
		bool loop;
		do {
			loop = false;
			for(CycleTask &task: this->timedTasks) {
				if (task.elapsedTime < currentTime) {
					if (task.task() == TaskReturnvalue::EXIT) {
						exit = true;
					}
					task.elapsedTime += task.cycleTime;
				}
				if (task.elapsedTime < currentTime) {
					loop = true;
				}
			}

		} while (loop);

		drawTask();

	} while (!exit);

}
