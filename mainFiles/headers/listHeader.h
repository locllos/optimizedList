#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define referTo(node) list->buffer[node]

#define LIST_ASSERT(list)     \
    if (!assertList(list)) assert(!"LIST OK")

typedef double elem_t;
typedef size_t virtual_ptr;

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

    virtual_ptr next;
    virtual_ptr prev;

    statusNode status;
};

struct List
{
    size_t size;
    size_t capacity;

    virtual_ptr head; //buffer[0] fict elem, his next points to first elem and his prev points to last elem
    virtual_ptr tail;

    Node* buffer;

    virtual_ptr free;

    bool is_optimized;
};

void Copy(void* value_to, const void* value_from, const size_t elem_size);

void initNode(List* list, virtual_ptr node);

void initNode(List* list, virtual_ptr node, elem_t value);

void initNode(List* list, virtual_ptr node, virtual_ptr prev, virtual_ptr next, elem_t value);

void constructList(List* list, size_t start_capacity);

List* newList(size_t start_capacity);

void freeMemory(List* list, virtual_ptr deletable_node);

virtual_ptr getFirst(List* list);

virtual_ptr getLast(List* list);

virtual_ptr getFreeSpace(List* list);

virtual_ptr getVirtualAddressByPos(List* list, size_t pos);

virtual_ptr insertBetween(List* list, virtual_ptr node_before, virtual_ptr node_after, elem_t value);

void pushBackList(List* list, elem_t value);

void pushForwardList(List* list, elem_t value);

void insertAfter(List* list, size_t pos_after, elem_t value);

void insertBefore(List* list, size_t pos_before, elem_t value);

void _service_delete_(List* list, virtual_ptr deletable_node);

void deleteNode(List* list, size_t pos);

void deleteHead(List* list);

void deleteTail(List* list);

void optimizeList(List* list);

void showListConsole(List* list, const char* reason);

void showNode(List* list, virtual_ptr node, const char* reason);

void showBuffer(List* list);

void destructList(List* list);

List* deleteList(List* list);

void logicListDump(List* list, const char* filename, const char* graph_filename);

void realListDump(List* list, const char* filename, const char* graph_filename);

void determineNodeStatus_Assistent(List* list, virtual_ptr cur_node);

void determineNodesStatuses_Cheif_Please(List* list);

void drawGraph(const char* filename, const char* graph_filename);

bool assertList(List* list);

void ListDumpReal(List* list);