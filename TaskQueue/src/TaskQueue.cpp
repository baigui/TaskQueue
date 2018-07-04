
//

#include <thread>
#include "TaskQueue.h"
#include "CommonFunc.h"

TaskQueue::TaskQueue(int precisionUsec)
		: _threadWork(false), _threadAlive(false),
		  _dealing(false), _precisionUsec(precisionUsec), _autoFlow(false),
          opaque(nullptr), taskQueueStart(nullptr), taskQueueStop(nullptr){
	initSleepFunc();
}

TaskQueue::~TaskQueue() {
	stopWork();
	clear();
	deinitSleepFunc();
}

void TaskQueue::scheduleTask(TaskItem *task) {
	if (task == nullptr)
		return;
	TaskItem *item = static_cast<TaskItem *>(getTail());
	if (item == nullptr) { //第一次添加任务，就直接添加，完成任务。
		insertTail(task);
	} else {
		//从后往前进行遍历
		lock();
		while (item && task->isInstancyThan(item)) {
			item = static_cast<TaskItem *>(item->getLast());
		}
		unlock();
		if (item == nullptr) {
			insertHead(task);
		} else {
			insertAfter(item, task);
		}
	}
	if (_autoFlow) {
		startWork();
	}
}
//instancy迫切性的意思
void TaskQueue::scheduleInstancyTask(TaskItem *task) {
	if (task) {
		task->setDealTime(MIN_INT64);
		scheduleTask(task);
	}
}

void TaskQueue::scheduleTaskNow(TaskItem *task) {
	if (task) {
		task->setDealTime(getCurrentTimeUsec());
		scheduleTask(task);
	}
}

void TaskQueue::scheduleTaskAfter(TaskItem *task, int64_t afterUsec) {
	if (task) {
		task->setDealTime(getCurrentTimeUsec() + afterUsec);
		scheduleTask(task);
	}
}

TaskItem *TaskQueue::getInstancyTask() {
	TaskItem *task = static_cast<TaskItem *>(getHead());
	if (task && task->getDealTime() <= getCurrentTimeUsec()) {
		remove(task);
		return task;
	}
	return nullptr;
}

void TaskQueue::removeTask(TaskItem *task) {
	remove(task);
}

bool TaskQueue::alive() {
	return _threadAlive;
}

bool TaskQueue::isDone() {
	return !_threadAlive;
}

void TaskQueue::waitDone() {
	while (_threadAlive) {
		sleepUs(_precisionUsec);
	}
}

void TaskQueue::clearTasks() {
	clear();
}

void TaskQueue::_thread_work(TaskQueue *tasks) {
	tasks->_threadAlive = true;
	//fprintf(stderr, "work thread start\n");
	if(tasks->taskQueueStart){
		tasks->taskQueueStart(tasks->opaque);
	}
	while (tasks->_threadWork) {
		TaskItem *task = tasks->getInstancyTask();
		if (task) {
			tasks->_dealing = true;
			if (task->dealTask()) {
				delete task;
			}
			tasks->_dealing = false;
		} else {
			if (tasks->_autoFlow && tasks->length() == 0) {
				tasks->_threadWork = false;
			} else {
				sleepUs(tasks->_precisionUsec);
			}
		}
	}
	if(tasks->taskQueueStop){
		tasks->taskQueueStop(tasks->opaque);
	}
	//fprintf(stderr, "work thread stop\n");
	tasks->_threadAlive = false;
}

void TaskQueue::startWork() {
    if(!_threadWork){  //这个大概是为了在工作过程中，就什么也不做，因为等下他会自动处理
        while (_threadAlive) { //如果有线程在工作，就先定时唤醒，
            sleepUs(_precisionUsec);
        }
        _threadWork = true;
        std::thread th(TaskQueue::_thread_work, this);
		th.detach();
        while(!_threadAlive){
            sleepUs(_precisionUsec);
        }
    }
}

void TaskQueue::stopWork(bool waitDone) {
	if (_threadWork) {
		_threadWork = false;
		if(waitDone) {
			while (_threadAlive) {
				sleepUs(_precisionUsec);
			}
		}
	}
}

void TaskQueue::setPrecision(int usec) {
	_precisionUsec = usec;
}

void TaskQueue::setAutoFlow(bool autoFlow) {
	_autoFlow = autoFlow;
}

TaskQueue *TaskQueue::defaultQueue() {
	if (_defaultQueue == nullptr) {
		_defaultQueue = new TaskQueue();
		_defaultQueue->setAutoFlow(true);
	}
	return _defaultQueue;
}

void TaskQueue::releaseDefaultQueue() {
	if (_defaultQueue) {
		delete _defaultQueue;
		_defaultQueue = nullptr;
	}
}

TaskQueue *TaskQueue::_defaultQueue = nullptr;
