
//

#ifndef PROJECT_GENERALDEQUE_H
#define PROJECT_GENERALDEQUE_H

#include <mutex>
#include "DequeItem.h"

class GeneralDeque {
public:
	GeneralDeque();

	virtual ~GeneralDeque();

public:
	/**
	 * @brief 在头部插入元素
	 *
	 * @param	item	要插入的元素
	 *
	 * @result	成功返回true,失败返回false
	 */
	bool insertHead(DequeItem *item);

	/**
	 * @brief 在尾部插入元素
	 *
	 * @param	item	要插入的元素
	 *
	 * @result	成功返回true,失败返回false
	 */
	bool insertTail(DequeItem *item);

	/**
	 * @brief 插入元素到某一元素后面
	 *
	 * @param	after	插入到该元素后面
	 * @param	item	要插入的元素
	 *
	 * @result	成功返回true,失败返回false
	 */
	bool insertAfter(DequeItem *after, DequeItem *item);

	/**
	 * @brief 插入元素到某一元素前面
	 *
	 * @param	before	插入到该元素前面
	 * @param	item	要插入的元素
	 *
	 * @result	成功返回true,失败返回false
	 */
	bool insertBefore(DequeItem *before, DequeItem *item);

	/**
	 * 将元素从链表中移除
	 *
	 * @param	item	要从链表中移除的元素
	 *
	 * @warning	该操作不会释放被移除的元素
	 *
	 * @result	成功返回true,失败返回false
	 */
	bool remove(DequeItem *item);

	/**
	 * @brief 清空链表
	 *
	 * @warning	该操作会自动释放所有元素
	 */
	void clear();

	/**
	 * @brief 获取链表头部第一个元素
	 *
	 * @result	成功返回元素指针,否则返回NULL
	 */
	DequeItem *getHead();

	/**
	 * @brief 获取链表尾部最后一个元素
	 *
	 * @result	成功返回元素指针,否则返回NULL
	 */
	DequeItem *getTail();

	/**
	 * @brief 获取链表长度
	 *
	 * @result	返回链表包含元素个数
	 */
	int length();

	/**
	 * @brief 为链表上锁,保证线程操作安全
	 */
	void lock();

	/**
	 * @brief 为链表解锁
	 */
	void unlock();

	/**
	 * @brief 打印队列，显示每个元素的标记，用于调试
	 */
	void printDeque();

private:
	DequeItem *_head;
	DequeItem *_tail;
	int _length;
	std::mutex _mutex;

};


#endif //PROJECT_GENERALDEQUE_H
