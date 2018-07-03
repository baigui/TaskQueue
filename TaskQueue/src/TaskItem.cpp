//
// Created by 张圣辉 on 16/8/27.
//
#include <thread>
#include "TaskItem.h"

TaskItem::TaskItem(std::function<bool(TaskItem *)> task, int64_t dealTime, bool async, char tag)
		: DequeItem(tag), _dealTime(dealTime), _taskResult(0), _async(async), _task(task) {
}

TaskItem::~TaskItem() {
}

bool TaskItem::isTimeToDeal(int64_t currentTime) {
	return currentTime >= _dealTime;
}

bool TaskItem::isInstancyThan(TaskItem *task) {
	if (task != nullptr) {
		return _dealTime < task->getDealTime();//比较自己的dealTime和task的执行时间,时间是相对时间还是绝对时间?
	}
	return false;
}

int64_t TaskItem::getDealTime() {
	return _dealTime;
}

void TaskItem::setDealTime(int64_t dealTime) {
	_dealTime = dealTime;
}

bool TaskItem::dealTask() {
	if (_task) {
		if (_async) {
			std::thread th(
					[this]() {
						if (_task(this)) {
							delete this;
						}
					}
			);
			th.detach();
			return false;
		} else {
			return _task(this);
		}
	}
	return false;
}

void TaskItem::setAsync(bool async) {
	_async = async;
}
