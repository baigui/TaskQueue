//
// Created by 张圣辉 on 16/8/26.
//

#ifndef PROJECT_DEQUEITEM_H
#define PROJECT_DEQUEITEM_H


class DequeItem {
public:
	DequeItem(char tag = ' ');

	virtual ~DequeItem();

public:
	/**
	 * @brief 获取上一个元素
	 */
	DequeItem *getLast();

	/**
	 * @brief 设置上一个元素
	 *
	 * @warning	用户不可直接调用该函数,可能导致链表结构受损
	 */
	void setLast(DequeItem *item);

	/**
	 * @brief 获取下一个元素
	 */
	DequeItem *getNext();

	/**
	 * @brief 设置下一个元素
	 *
	 * @warning	用户不可直接调用该函数,可能导致链表结构受损
	 */
	void setNext(DequeItem *item);

	/**
	 * @brief 检查是否在链表当中
	 *
	 * @result 是则返回true,否则返回false
	 */
	bool isInDeque();

	/**
	 * @brief 设置是否在链表当中
	 *
	 * @param	inDeque	指定元素是否在链表当中
	 */
	void setInDeque(bool inDeque);

	/**
	 * @brief 设置当链表清空时是否自动释放该元素
	 *
	 * @param	autoRelease 指定是否自动释放，默认为true
	 *
	 * @warning	若不对元素调用该函数，则默认自动释放
	 */
	void setAutoRelease(bool autoRelease = true);

	/**
	 * @brief 查询是否应该在链表清空时自动释放该元素
	 *
	 * @result	自动释放返回true，否则返回false
	 */
	bool shouldAutoRelease();

	/**
	 * @brief 设置标记
	 *
	 * @param	tag		要设置标记
	 */
	void setTag(char tag);

	/**
	 * @brief 获取标记
	 */
	char getTag();

private:
	DequeItem *_last;
	DequeItem *_next;
	bool _inDeque;
	bool _autoRelease;
	char _tag;
};


#endif //PROJECT_DEQUEITEM_H
