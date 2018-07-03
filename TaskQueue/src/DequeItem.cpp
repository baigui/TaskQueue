//
// Created by 张圣辉 on 16/8/26.
//

#include "DequeItem.h"
#include <stdlib.h>

DequeItem::DequeItem(char tag)
		: _last(nullptr), _next(nullptr), _inDeque(false), _autoRelease(true), _tag(tag) {

}

DequeItem::~DequeItem() {

}

DequeItem *DequeItem::getLast() {
	return _last;
}

void DequeItem::setLast(DequeItem *item) {
	_last = item;
}

DequeItem *DequeItem::getNext() {
	return _next;
}

void DequeItem::setNext(DequeItem *item) {
	_next = item;
}

bool DequeItem::isInDeque() {
	return _inDeque;
}

void DequeItem::setInDeque(bool inDeque) {
	_inDeque = inDeque;
}

void DequeItem::setAutoRelease(bool autoRelease) {
	_autoRelease = autoRelease;
}

bool DequeItem::shouldAutoRelease() {
	return _autoRelease;
}

void DequeItem::setTag(char tag) {
	_tag = tag;
}

char DequeItem::getTag() {
	return _tag;
}
