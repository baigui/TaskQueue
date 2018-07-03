//
// Created by 张圣辉 on 16/8/26.
//

#ifndef PROJECT_VFTASKQUEUE_H
#define PROJECT_VFTASKQUEUE_H

#include "GeneralDeque.h"
#include "TaskItem.h"

extern "C" {

typedef void (*TaskQueueStartCallback)(void *opaque);
typedef void (*TaskQueueStopCallback)(void *opaque);

};

class TaskQueue : private GeneralDeque {
public:
	TaskQueue(int precisionUsec = 1000);

	virtual ~TaskQueue();

public:
	void *opaque;
	TaskQueueStartCallback taskQueueStart;
	TaskQueueStopCallback taskQueueStop;

public:
	void scheduleTask(TaskItem *task);

	void scheduleInstancyTask(TaskItem *task);

	void scheduleTaskNow(TaskItem *task);

	void scheduleTaskAfter(TaskItem *task, int64_t afterUsec);

	TaskItem *getInstancyTask();

	void removeTask(TaskItem *task);

	bool alive();

	bool isDone();

	void waitDone();

	void clearTasks();

	void startWork();

	void stopWork(bool waitDone = true);

	void setPrecision(int usec);

	void setAutoFlow(bool autoFlow);

	static TaskQueue *defaultQueue();

	static void releaseDefaultQueue();

private:

	static void _thread_work(TaskQueue *tasks);

    bool _threadWork;
    
	bool _threadAlive;

	bool _dealing;

	int _precisionUsec;

	bool _autoFlow;

	static TaskQueue *_defaultQueue;
};


#endif //PROJECT_VFTASKQUEUE_H
