#ifndef DEQUEBASE_HPP
#define DEQUEBASE_HPP

#include <iostream>
#include <stdio.h>
#include <cstring>

#define STR(t) #t

#define CAT(t) "Deque_" STR(t)

#define Deque_DEFINE(t) \
typedef struct deque_##t Deque_##t; \
typedef struct it_##t Deque_##t##_Iterator; \
  \
struct it_##t{ \
	int currentIndex; \
	Deque_##t *currentDeque; \
	int beginIndex; \
	int endIndex; \
	int maxSize; \
	void (*inc)(Deque_##t##_Iterator* it); \
	void (*dec)(Deque_##t##_Iterator* it); \
	t &(*deref)(Deque_##t##_Iterator* it); \
}; \
  \
struct deque_##t{ \
	int sizeInt; \
	int maxSize; \
	t *arr; \
	int beginIndex; \
	int endIndex; \
	char type_name[sizeof CAT(t)] = CAT(t); \
	bool (*less)(const t &i1, const t &i2); \
	Deque_##t##_Iterator (*begin)(Deque_##t *thisDeque); \
	Deque_##t##_Iterator (*end)(Deque_##t *thisDeque); \
	void (*reallocate)(Deque_##t *thisDeque); \
	t &(*front)(Deque_##t *thisDeque); \
	t &(*back)(Deque_##t *thisDeque); \
	int (*size)(Deque_##t *thisDeque); \
	bool (*empty)(Deque_##t *thisDeque); \
	void (*push_back)(Deque_##t *thisDeque, t toInsert); \
	void (*push_front)(Deque_##t *thisDeque, t toInsert); \
	void (*pop_back)(Deque_##t *thisDeque); \
	void (*pop_front)(Deque_##t *thisDeque); \
	t &(*at)(Deque_##t *thisDeque, int index); \
	void (*clear)(Deque_##t *thisDeque); \
	void (*dtor)(Deque_##t *thisDeque); \
}; \
 \
void inc(Deque_##t##_Iterator* it){ \
	if(it->currentIndex == it->endIndex){ \
		it->currentIndex = -1; /*sentinel value equivalent to end*/ \
	} \
	else{ \
		it->currentIndex = (it->currentIndex + 1) % it->maxSize; \
	} \
} \
void dec(Deque_##t##_Iterator* it){ \
	if(it->currentIndex == -1){ \
		it->currentIndex = it->endIndex; \
	} \
	else{ \
		it->currentIndex = (it->currentIndex - 1 + it->maxSize) % it->maxSize; \
	} \
} \
t &deref(Deque_##t##_Iterator* it){ \
	return it->currentDeque->arr[it->currentIndex]; \
} \
 \
bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it1, Deque_##t##_Iterator it2){ \
	if(it1.currentDeque != it2.currentDeque) {return false;} \
	return it1.currentIndex == it2.currentIndex; \
} \
bool Deque_##t##_equal(Deque_##t thisDeque1, Deque_##t thisDeque2){ \
	if(thisDeque1.sizeInt != thisDeque2.sizeInt) {return false;} \
	for(int i=0; i<thisDeque1.sizeInt; i++){ \
		t d1 = thisDeque1.arr[(thisDeque1.beginIndex + i) % thisDeque1.maxSize]; \
		t d2 = thisDeque2.arr[(thisDeque2.beginIndex + i) % thisDeque2.maxSize]; \
		if(thisDeque1.less(d1, d2) || thisDeque1.less(d2, d1)) {return false;} \
	} \
	return true; \
} \
 \
Deque_##t##_Iterator begin(Deque_##t *thisDeque){ \
	Deque_##t##_Iterator ret; \
	ret.inc = &inc; \
	ret.dec = &dec; \
	ret.deref = &deref; \
	ret.maxSize = thisDeque->maxSize; \
	ret.currentDeque = thisDeque; \
	ret.beginIndex = thisDeque->beginIndex; \
	ret.endIndex = thisDeque->endIndex; \
	ret.currentIndex = ret.beginIndex; \
	return ret; \
} \
Deque_##t##_Iterator end(Deque_##t *thisDeque){ \
	Deque_##t##_Iterator ret; \
	ret.inc = &inc; \
	ret.dec = &dec; \
	ret.deref = &deref; \
	ret.maxSize = thisDeque->maxSize; \
	ret.currentDeque = thisDeque; \
	ret.beginIndex = thisDeque->beginIndex; \
	ret.endIndex = thisDeque->endIndex; \
	ret.currentIndex = -1; \
	return ret; \
} \
void reallocate(Deque_##t *thisDeque){ \
	/*size is full, realloc and copy*/ \
	t *tempArr1 = thisDeque->arr; \
	t *tempArr2 = (t *)malloc(sizeof(t) * thisDeque->maxSize*2); \
	int copyIndex = 0; \
	for(auto it = thisDeque->begin(thisDeque); !Deque_##t##_Iterator_equal(it, thisDeque->end(thisDeque)); it.inc(&it)){ \
		tempArr2[copyIndex] = it.deref(&it); \
		copyIndex++; \
	} \
	thisDeque->beginIndex = 0; \
	thisDeque->endIndex = thisDeque->sizeInt-1; \
	thisDeque->sizeInt = thisDeque->maxSize; \
	thisDeque->maxSize *= 2; \
	thisDeque->arr = tempArr2; \
	free (tempArr1); \
} \
t &front(Deque_##t *thisDeque){ \
	return thisDeque->arr[thisDeque->beginIndex]; \
} \
t &back(Deque_##t *thisDeque){ \
	return thisDeque->arr[thisDeque->endIndex]; \
} \
int size(Deque_##t *thisDeque){ \
	return thisDeque->sizeInt; \
} \
bool empty(Deque_##t *thisDeque){ \
	return thisDeque->sizeInt == 0; \
} \
void push_back(Deque_##t *thisDeque, t toInsert){ \
	if(thisDeque->sizeInt == 0){ \
		thisDeque->endIndex = 0; \
		thisDeque->beginIndex = 0; \
		thisDeque->arr[0] = toInsert; \
		thisDeque->sizeInt++; \
		return; \
	} \
	if(thisDeque->sizeInt == thisDeque->maxSize){ \
		thisDeque->reallocate(thisDeque); \
	} \
	thisDeque->endIndex = (thisDeque->endIndex + 1) % thisDeque->maxSize; \
	thisDeque->arr[thisDeque->endIndex] = toInsert; \
	thisDeque->sizeInt++; \
} \
void push_front(Deque_##t *thisDeque, t toInsert){ \
	if(thisDeque->sizeInt == 0){ \
		thisDeque->endIndex = 0; \
		thisDeque->beginIndex = 0; \
		thisDeque->arr[0] = toInsert; \
		thisDeque->sizeInt++; \
		return; \
	} \
	if(thisDeque->sizeInt == thisDeque->maxSize){ \
		thisDeque->reallocate(thisDeque); \
	} \
	thisDeque->beginIndex = ((thisDeque->beginIndex - 1) + thisDeque->maxSize) % thisDeque->maxSize; \
	thisDeque->arr[thisDeque->beginIndex] = toInsert; \
	thisDeque->sizeInt++; \
} \
void pop_back(Deque_##t *thisDeque){ \
	thisDeque->sizeInt--; \
	thisDeque->endIndex = (thisDeque->endIndex -1 + thisDeque->maxSize) % thisDeque->maxSize; \
} \
void pop_front(Deque_##t *thisDeque){ \
	thisDeque->sizeInt--; \
	thisDeque->beginIndex = (thisDeque->beginIndex + 1) % thisDeque->maxSize; \
} \
t &at(Deque_##t *thisDeque, int index){ \
	return thisDeque->arr[(thisDeque->beginIndex + index) % thisDeque->maxSize]; \
} \
void clear(Deque_##t *thisDeque){ \
	t *temp = thisDeque->arr; \
	thisDeque->arr = (t *)malloc(sizeof(t) * 2); \
	free (temp); \
	thisDeque->beginIndex = 0; \
	thisDeque->endIndex = 0; \
	thisDeque->sizeInt = 0; \
	thisDeque->maxSize = 4; \
} \
void dtor(Deque_##t *thisDeque){ \
	free (thisDeque->arr); \
} \
 \
void Deque_##t##_ctor(Deque_##t *deq, bool (*less)(const t &i1, const t &i2)){ \
	deq->arr = (t *)malloc(sizeof(t) * 2); \
	deq->maxSize = 2; \
	deq->sizeInt = 0; \
	deq->beginIndex = 0; \
	deq->endIndex = 0; \
	deq->begin = &begin; \
	deq->end = &end; \
	deq->reallocate = &reallocate; \
	deq->front = &front; \
	deq->back = &back; \
	deq->size = &size; \
	deq->empty = &empty; \
	deq->push_back = &push_back; \
	deq->push_front = &push_front; \
	deq->pop_back = &pop_back; \
	deq->pop_front = &pop_front; \
	deq->at = &at; \
	deq->less = less; \
	deq->clear = &clear; \
	deq->dtor = &dtor; \
} \

#endif