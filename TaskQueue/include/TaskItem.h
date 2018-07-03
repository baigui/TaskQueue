//
//

#ifndef PROJECT_VFTASK_H
#define PROJECT_VFTASK_H

#include <functional>
#include "DequeItem.h"

class TaskItem : public DequeItem {
public:
	TaskItem(std::function<bool(TaskItem *)> task, int64_t dealTime = 0, bool async = false, char tag = ' ');

	virtual ~TaskItem();

public:
	/**
	 * @brief 判断是否已经到任务执行时间
	 *
	 * @result	已到执行时间,返回true,否则返回false
	 */
	bool isTimeToDeal(int64_t currentTime);

	/**
	 * @brief 判断是否比指定任务更紧迫
	 *
	 * @result	是则返回true,否则返回false
	 */
	bool isInstancyThan(TaskItem *task);

	/**
	 * @brief 获取处理时间
	 *
	 * @result	返回处理时间
	 */
	int64_t getDealTime();

	/**
	 * @brief 设置处理时间
	 *
	 * @param	dealTime	要设置的处理时间
	 */
	void setDealTime(int64_t dealTime);

	/**
	 * @brief 执行任务处理
	 *
	 * @warning	任务队列将根据返回值决定是否自动释放该任务,返回true则自动释放
	 *
	 * @result	任务处理结果
	 */
	virtual bool dealTask();

	/**
	 * @brief 设置是否异步执行
	 *
	 * @param	async	指定是否异步执行
	 */
	void setAsync(bool async);

private:
	int64_t _dealTime;
	int _taskResult;
	bool _async;
	std::function<bool(TaskItem *)> _task;
};


#endif //PROJECT_VFTASK_H
