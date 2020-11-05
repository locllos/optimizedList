#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define referTo(idx) list->buffer[idx]

#define listReferTo(list, idx) list->buffer[idx]

#define LIST_ASSERT(list)     \
    if (!assertList(list)) assert(!"LIST OK")

typedef double elem_t;

enum statusNode
{   
    NEW = 0, 
    FREE,
    BROKEN,
    OK,
};

struct Node
{
    elem_t value;

    size_t next;
    size_t prev;

    statusNode status;
};

struct List
{
    size_t size;
    size_t capacity;

    size_t head;
    size_t tail;

    Node* buffer;

    size_t free_head;

    bool is_optimized;
};



void Copy(void* value_to, const void* value_from, const size_t elem_size);

void initNode(List* list, size_t node);

void initNode(List* list, size_t node, elem_t value);

void initNode(List* list, size_t node, size_t prev, size_t next, elem_t value);

void constructList(List* list, size_t start_capacity);

List* newList(size_t start_capacity);

void freeNode(List* list, size_t deletable_idx);

size_t getFirst(List* list);

size_t getLast(List* list);

size_t getFreeNode(List* list);

size_t getIndexByNumber(List* list, size_t number);

size_t insertBetween(List* list, size_t idx_before, size_t idx_after, elem_t value);

void pushBackList(List* list, elem_t value);

void pushForwardList(List* list, elem_t value);

void insertAfter(List* list, size_t number_after, elem_t value);

void insertBefore(List* list, size_t number_before, elem_t value);

void service_delete(List* list, size_t deletable_idx);

void deleteNode(List* list, size_t number);

void deleteHead(List* list);

void deleteTail(List* list);

void slowOptimizeList(List* list);

void showListConsole(List* list, const char* reason);

void showNode(List* list, size_t node, const char* reason);

void showBuffer(List* list);

void destructList(List* list);

List* deleteList(List* list);

void logicListDump(List* list, const char* filename, const char* graph_filename);

void realListDump(List* list, const char* filename, const char* graph_filename);

void determineNodeStatus_Assistent(List* list, size_t cur_idx);

void determineNodesStatuses_Cheif_Please(List* list);

void drawGraph(const char* filename, const char* graph_filename);

bool assertList(List* list);

