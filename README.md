# TaskQueue
这是一个消息队列处理工具库，里面有一些测试工具，主要的代码都在TaskQueue目下。

# 关键部分
异步是通过Select来实现的。这里思路很简单我们都是通过std::thread来实现了异步操作，但是异步操作的sleep却是通过select的阻塞式来完成的，我们这里为了方便没有设置特定的阻塞之间，都是通过一个特定的时间值来确定的，并且通过特定的值来实现了queue的同时只能一个task执行，

消息队列是通过继承GeneralDeque来维护消息队列，但是这里完全可以用STD的deque来直接维持队列计数，没必要这么麻烦。

这里大概就这么多，在有消息执行的时候，就通过
```
std::thread th(TaskQueue::_thread_work, this);
th.detach();
```
来开启新的线程，调用_thread_work函数，实现异步操作，具体的操作都是在
```
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
```
这里处理任务的，如果task可以执行，就调用```task->dealTask()```来执行task，如果当前没有任务执行，就直接调用sleepUS来进行线程休眠。