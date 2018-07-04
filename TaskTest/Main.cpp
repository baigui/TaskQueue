#include <Winsock.h>
#include <string>
#include <iostream>
#include <TaskQueue.h>
#include <deque>

using namespace std;


#define BUFSIZE 64
#define PORT 9999

int main()
{

	std::mutex _muc;
	TaskQueue *_tasks = new TaskQueue();
	TaskItem *item = new TaskItem(
		[&_muc](TaskItem * item) {
		_muc.lock();
		printf("now pid is %d\n", GetCurrentProcessId());
		Sleep(2000);
		_muc.unlock();
		cout << "我愛大司馬" << endl;
		return true;

	}, 0, false, 'D'
	);
	_tasks->scheduleTask(item);
	_tasks->startWork();
	_muc.lock();
	printf("now pid is %d\n", GetCurrentProcessId());
	_muc.unlock();

	_tasks->scheduleTask(new TaskItem([&_muc](TaskItem * item) {
		printf("now pid is %d\n", std::this_thread::get_id());
		Sleep(2000);
		cout << "我愛大司馬" << endl;
		return true;

	}, 0, false, 'D'));
	_tasks->startWork();
	getchar();
	std::deque<int> d = { 7, 5, 16, 8 };
	return 0;
}
