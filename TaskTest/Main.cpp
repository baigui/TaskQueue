#include <Winsock.h>
#include <string>
#include <iostream>
#include <TaskQueue.h>

using namespace std;


#define BUFSIZE 64
#define PORT 9999

int main()
{

	TaskQueue *_tasks = new TaskQueue();

	TaskItem *item = new TaskItem(
		[](TaskItem * item) {
		cout << "�Ґ۴�˾�R" << endl;
		return true;

	}, 0, false, 'D'
	);

	_tasks->scheduleTask(item);
	_tasks->startWork();
	getchar();
	return 0;
}
