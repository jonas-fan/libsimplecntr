#include <list.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct list_node_t list_node_t;

struct list_node_t
{
    void *data;
    unsigned int data_size;
    list_node_t *previous;
    list_node_t *next;
};

struct list_t
{
    unsigned int size;
    list_node_t *head;
    list_node_t *tail;
};

static inline list_node_t * list_element_create(const void *data, unsigned int data_size)
{
    list_node_t *node = (list_node_t *)malloc(sizeof(list_node_t) + data_size);

    if (node) {
        memset(node, 0, sizeof(*node));

        node->data = (uint8_t *)node + sizeof(list_node_t);
        node->data_size = data_size;

        memcpy(node->data, data, data_size);
    }

    return node;
}

static inline void list_element_destroy(list_node_t *node)
{
    free(node);
}

static inline list_node_t * list_at_(const list_t *list, unsigned int index)
{
    if (index >= list->size) {
        return NULL;
    }

    list_node_t *node = list->head;

    while (index--) {
        node = node->next;
    }

    return node;
}


/**
 *    Public methods
 */

list_t * list_create(void)
{
    list_t *list = (list_t *)malloc(sizeof(list_t));

    if (list) {
        memset(list, 0, sizeof(*list));
    }

    return list;
}

void list_destroy(list_t *list)
{
    list_node_t *node = list->head;

    while (node) {
        list_node_t *next = node->next;

        list_element_destroy(node);

        node = next;
    }

    free(list);
}

unsigned int list_size(const list_t *list)
{
    return list->size;
}

int list_at(const list_t *list, unsigned int index, void *data, unsigned int data_size)
{
    const list_node_t *node = list_at_(list, index);

    if (!node) {
        return -1;
    }

    memcpy(data, node->data, data_size);

    return 0;
}

int list_front(const list_t *list, void *data, unsigned int data_size)
{
    if (!list->head) {
        return -1;
    }

    memcpy(data, list->head->data, data_size);

    return 0;
}

int list_back(const list_t *list, void *data, unsigned int data_size)
{
    if (!list->tail) {
        return -1;
    }

    memcpy(data, list->tail->data, data_size);

    return 0;
}

int list_insert(list_t *list, unsigned int index, const void *data, unsigned int data_size)
{
    list_node_t *node = list_at_(list, index);

    if (!node) {
        return -1;
    }

    list_node_t *new_node = list_element_create(data, data_size);

    if (!new_node) {
        return -1;
    }

    new_node->next = node;
    new_node->previous = node->previous;

    if (list->head == node) {
        list->head = new_node;
    }
    else {
        node->previous->next = new_node;
    }

    node->previous = new_node;

    ++list->size;

    return 0;
}

int list_erase(list_t *list, unsigned int index)
{
    list_node_t *node = list_at_(list, index);

    if (!node) {
        return -1;
    }

    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    }
    else if (node == list->head) {
        node->next->previous = NULL;
        list->head = node->next;
    }
    else if (node == list->tail) {
        node->previous->next = NULL;
        list->tail = node->previous;
    }
    else {
        node->next->previous = node->previous;
        node->previous->next = node->next;
    }

    --list->size;

    list_element_destroy(node);

    return 0;
}

int list_push_front(list_t *list, const void *data, unsigned int data_size)
{
    list_node_t *new_node = list_element_create(data, data_size);

    if (!new_node) {
        return -1;
    }

    if (list->head) {
        new_node->next = list->head;
        list->head->previous = new_node;
        list->head = new_node;
    }
    else {
        list->head = new_node;
        list->tail = new_node;
    }

    ++list->size;

    return 0;
}

int list_pop_front(list_t *list)
{
    if (!list->size) {
        return -1;
    }

    list_node_t *node = list->head;

    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    }
    else {
        list->head = node->next;
        list->head->previous = NULL;
    }

    --list->size;

    list_element_destroy(node);

    return 0;
}

int list_push_back(list_t *list, const void *data, unsigned int data_size)
{
    list_node_t *new_node = list_element_create(data, data_size);

    if (!new_node) {
        return -1;
    }

    if (list->tail) {
        new_node->previous = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    else {
        list->head = new_node;
        list->tail = new_node;
    }

    ++list->size;

    return 0;
}

int list_pop_back(list_t *list)
{
    if (!list->size) {
        return -1;
    }

    list_node_t *element = list->tail;

    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    }
    else {
        list->tail = element->previous;
        list->tail->next = NULL;
    }

    --list->size;

    list_element_destroy(element);

    return 0;
}

int list_find(const list_t *list, const void *data, unsigned int data_size)
{
    int index = 0;

    list_node_t *node = list->head;

    while (node && memcmp(node->data, data, data_size)) {
        node = node->next;

        ++index;
    }

    return (node)?  index : -1;
}

int list_find_if(const list_t *list,
                 const void *search,
                 unsigned int search_size,
                 int (*compare)(const void *data, unsigned int data_size, const void *search, unsigned int search_size))
{
    if (!compare) {
        return -1;
    }

    int index = 0;

    list_node_t *node = list->head;

    while (node && compare(node->data, node->data_size, search, search_size)) {
        node = node->next;

        ++index;
    }

    return (node)?  index : -1;
}

void list_for_each(list_t *list,
                   void (*func)(unsigned index, void *data, unsigned int data_size, void *user_data),
                   void *user_data)
{
    if (!func) {
        return;
    }

    unsigned int index = 0;

    list_node_t *node = list->head;

    while (node) {
        func(index, node->data, node->data_size, user_data);

        node = node->next;

        ++index;
    }
}