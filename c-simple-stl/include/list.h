#ifndef SIMPLE_LIST_H_
#define SIMPLE_LIST_H_

#include <list_element.h>

/**
 *    Simple List
 *
 *    @version 0.0.2
 *    @date    2014/06/18
 *    @author  Jian <jianfan.tw@gmail.com>
 */

typedef struct list_t
{
    unsigned int size;
    ListElement *head;
    ListElement *tail;
} List;

List * list_create(void);
void list_destroy(List *list);

unsigned int list_size(const List *list);

void * list_at(const List *list, unsigned int index);
void * list_front(const List *list);
void * list_back(const List *list);

void list_insert(List *list, unsigned int index, void *data);
void list_erase(List *list, unsigned int index);
void list_push_front(List *list, void *data);
void list_pop_fornt(List *list);
void list_push_back(List *list, void *data);
void list_pop_back(List *list);

#endif  // SIMPLE_LIST_H_