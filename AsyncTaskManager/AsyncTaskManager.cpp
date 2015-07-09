#include "stdafx.h"

#include <conio.h>

#include "TaskManager.h"

class SimpleTask : public Task {
public:
	SimpleTask(unsigned id, TaskEventsBroadcaster *renderStatistics) : Task(id, renderStatistics), dummy(0.0f) {}
	void run() {
		tasksStatistics->taskStarted(id);
		std::cout << "Running task (" << id << ")" << std::endl;
		// Perform some heavy computation...
		for (int i = 0; i < 1000000; ++i) {
			dummy += sqrtf(i);
		}
		tasksStatistics->taskEnded(id);
	}

private:
	float dummy;
};

int _tmain(int argc, _TCHAR* argv[]) {
	TaskManager *taskManager = new TaskManager();

	const unsigned maxTasks = 1000;
	for (unsigned i = 0; i < maxTasks; ++i) {
		Task *task = new SimpleTask(i, taskManager->getTaskEventsBroadcaster());
		taskManager->postTask(task);
	}

	taskManager->execute();
	while (taskManager->getExecutionState() != TasksExecutionState::FINISHED) {
		//std::cout << "Running: " << taskManager->getProgress() << std::endl;
		int c = _getch();
		if (c == 'p') {
			std::cout << "Paused" << std::endl;
			taskManager->pause();
		} else if (c == 'r') {
			std::cout << "Resumed" << std::endl;
			taskManager->resume();
		}
	}

	delete taskManager;
	return 0;
}
