
//

#include "GeneralDeque.h"

//#define DEBUG_LOG
GeneralDeque::GeneralDeque() {
	_head = nullptr;
	_tail = nullptr;
	_length = 0;
}

GeneralDeque::~GeneralDeque() {
	clear();
}

bool GeneralDeque::insertHead(DequeItem *item) {
	return insertBefore(nullptr, item);
}

bool GeneralDeque::insertTail(DequeItem *item) {
	return insertAfter(nullptr, item);
}

bool GeneralDeque::insertAfter(DequeItem *after, DequeItem *item) {
	if (item && !item->isInDeque()) {
		lock();
		if (after == nullptr) {
			after = _tail;
		}
		if (after == nullptr) {
			item->setLast(nullptr);
			item->setNext(nullptr);
			_head = item;
			_tail = item;
		} else if (after->isInDeque()) {
			item->setLast(after);
			item->setNext(after->getNext());
			after->setNext(item);
			if (item->getNext()) {
				item->getNext()->setLast(item);
			}
			if (after == _tail) {
				_tail = item;
			}
		} else {
			unlock();
			return false;
		}
		item->setInDeque(true);
		_length++;
		unlock();
#ifdef DEBUG_LOG
		printf("add item %c :", item->getTag());
		printDeque();
#endif
		return true;
	} else {
		return false;
	}
}

bool GeneralDeque::insertBefore(DequeItem *before, DequeItem *item) {
	if (item && !item->isInDeque()) {
		lock();
		if (before == nullptr) {
			before = _head;
		}
		if (before == nullptr) {
			item->setLast(nullptr);
			item->setNext(nullptr);
			_head = item;
			_tail = item;
		} else if (before->isInDeque()) {
			item->setLast(before->getLast());
			item->setNext(before);
			before->setLast(item);
			if (item->getLast()) {
				item->getLast()->setNext(item);
			}
			if (before == _head) {
				_head = item;
			}
		} else {
			unlock();
			return false;
		}
		item->setInDeque(true);
		_length++;
		unlock();
#ifdef DEBUG_LOG
		printf("Add item %c :", item->getTag());
		printDeque();
#endif
		return true;
	} else {
		return false;
	}
}

bool GeneralDeque::remove(DequeItem *item) {
	if (item && item->isInDeque()) {
		lock();
		if (item->getLast() != nullptr) {
			item->getLast()->setNext(item->getNext());
		} else {
			_head = item->getNext();
		}
		if (item->getNext() != nullptr) {
			item->getNext()->setLast(item->getLast());
		} else {
			_tail = item->getLast();
		}
		item->setLast(nullptr);
		item->setNext(nullptr);
		item->setInDeque(false);
		_length--;
		unlock();
#ifdef DEBUG_LOG
		printf("Del item %c :", item->getTag());
		printDeque();
#endif
		return true;
	} else {
		return false;
	}
}

void GeneralDeque::clear() {
	lock();
	DequeItem *item = _head;
	while (item) {
		DequeItem *nitem = item;
		item = item->getNext();
		nitem->setLast(nullptr);
		nitem->setNext(nullptr);
		nitem->setInDeque(false);
		if (nitem->shouldAutoRelease()) {
			delete nitem;
		}
	}
	_head = nullptr;
	_tail = nullptr;
	_length = 0;
	unlock();
}

DequeItem *GeneralDeque::getHead() {
	return _head;
}

DequeItem *GeneralDeque::getTail() {
	return _tail;
}

int GeneralDeque::length() {
	return _length;
}

void GeneralDeque::lock() {
	_mutex.lock();
}

void GeneralDeque::unlock() {
	_mutex.unlock();
}

void GeneralDeque::printDeque() {
	lock();
	printf("[ ");
	DequeItem *item = _head;
	while (item) {
		printf("%c", item->getTag());
		item = item->getNext();
		if (item) {
			printf(" -> ");
		}
	}
	printf(" ]\n");
	unlock();
}
