#include "headers/listHeader.h"

const char* DOT_COMMAND = "dot -Tjpg ";

const char* STATUS_COLORS[] = 
{
    "yellow",
    "white",
    "red",
    "green"
};


void initNode(List* list, size_t idx)
{
    referTo(idx).next = 0;
    referTo(idx).prev = 0;
    referTo(idx).value = NAN;
    referTo(idx).status = NEW;
}

void initNode(List* list, size_t idx, elem_t value)
{
    referTo(idx).next = 0;
    referTo(idx).prev = 0;
    referTo(idx).value = value;
    referTo(idx).status = NEW;
}

void initNode(List* list, size_t idx, size_t prev, size_t next, elem_t value)
{
    referTo(idx).prev = prev;
    referTo(idx).next = next;
    referTo(idx).value = value;
    referTo(idx).status = NEW;
}

void constructList(List* list, size_t start_capacity)
{
    list->size = 0;
    list->capacity = start_capacity + 1;
    list->free_head = 1;
    list->is_optimized = false;
    list->head = 1;
    list->tail = 1;

    list->buffer = (Node*)calloc(list->capacity, sizeof(Node));
    initNode(list, 0, 0, 0, NAN);
    referTo(0).status = OK;

    size_t cur_idx = 1;
    for (; cur_idx < list->capacity; ++cur_idx)
    {
        initNode(list, cur_idx, -1, cur_idx + 1, NAN);
        referTo(cur_idx).status = FREE;
    }
    initNode(list, cur_idx - 1, -1, 0, NAN);
    referTo(cur_idx - 1).status = FREE;
}

List* newList(size_t start_capacity)
{
    List* list = (List*)calloc(1, sizeof(List));

    constructList(list, start_capacity);

    return list;
}

void freeNode(List* list, size_t deletable_node)
{
    initNode(list, deletable_node, -1, list->free_head, NAN);
    referTo(deletable_node).status = FREE;

    list->free_head = deletable_node;
}

size_t getFirst(List* list)
{
    return list->head;
}

size_t getLast(List* list)
{
    return list->tail;
}

size_t getFreeNode(List* list)
{   
    assert(list->size + 1 < list->capacity);

    size_t free_head = list->free_head;

    list->free_head = referTo(free_head).next;

    return free_head;
}

size_t getIndexByNumber(List* list, size_t number)
{
    if (list->is_optimized) return number;

    size_t cur_idx = list->head;
    for (size_t i = 1; i < number; ++i)
    {
        cur_idx = referTo(cur_idx).next;
    }
    return cur_idx;
}

size_t insertBetween(List* list, size_t idx_before, size_t idx_after, elem_t value)
{   
    LIST_ASSERT(list);
    assert(idx_before >= 0);
    assert(idx_before < list->capacity);
    assert(idx_after >= 0);
    assert(idx_after < list->capacity);
    assert(referTo(idx_after).status != BROKEN);

    size_t insertable_node = getFreeNode(list);

    initNode(list, insertable_node, idx_before, idx_after, value);
    referTo(idx_before).next = insertable_node;
    referTo(idx_after).prev = insertable_node;

    ++list->size;

    determineNodeStatus_Assistent(list, insertable_node);

    LIST_ASSERT(list);

    return insertable_node;
}

void pushBackList(List* list, elem_t value)
{
    list->tail = insertBetween(list, list->tail, 0, value);
    //Так как в одноэлементном списке голова и хвост указывают на одно и то же, то меняем их вместе
    if (list->size == 1)
    {
        list->head = list->tail;
        referTo(list->tail).prev = 0;
    }
    referTo(list->tail).next = 0;
}

void pushForwardList(List* list, elem_t value)
{
    list->head = insertBetween(list, 0, list->head, value);
    //Так как в одноэлементном списке голова и хвост указывают на одно и то же, то меняем их вместе 
    if (list->size == 1)
    {
        list->head = list->tail;
        referTo(list->head).next = 0;
    }
    referTo(list->head).prev = 0;
}

void insertAfter(List* list, size_t number_after, elem_t value)
{   
    size_t idx_after = getIndexByNumber(list, number_after);

    insertBetween(list, idx_after, referTo(idx_after).next, value);
}

size_t realInsertAfter(List* list, size_t idx_after, elem_t value)
{   
    assert(referTo(idx_after).status != FREE);

    return insertBetween(list, idx_after, referTo(idx_after).next, value);
}

void insertBefore(List* list, size_t number_before, elem_t value)
{
    size_t idx_before = getIndexByNumber(list, number_before);
    
    insertBetween(list, referTo(idx_before).prev, idx_before, value);
}

size_t realInsertBefore(List* list, size_t idx_before, elem_t value)
{    
    assert(referTo(idx_before).status != FREE);

    return insertBetween(list, referTo(idx_before).prev, idx_before, value);
}

void service_delete(List* list, size_t deletable_idx)
{
    LIST_ASSERT(list);
    assert(deletable_idx > 0);
    assert(deletable_idx < list->capacity);

    referTo(referTo(deletable_idx).next).prev = referTo(deletable_idx).prev;
    referTo(referTo(deletable_idx).prev).next = referTo(deletable_idx).next;

    freeNode(list, deletable_idx);

    --list->size;

    LIST_ASSERT(list);
}

void deleteNode(List* list, size_t number)
{   
    assert(number > 0);
    assert(number < list->size + 1);

    size_t deletable_node = getIndexByNumber(list, number);

    if (deletable_node == list->head)
    {
        list->head = referTo(deletable_node).next;
    }
    else if (deletable_node == list->tail)
    {
        list->tail = referTo(deletable_node).prev;
    }
    service_delete(list, deletable_node);
}

void deleteHead(List* list)
{
    size_t deletable_idx = list->head;

    list->head = referTo(deletable_idx).next;

    service_delete(list, deletable_idx);
}

void deleteTail(List* list)
{
    size_t deletable_idx = list->tail;

    list->tail = referTo(deletable_idx).prev;

    service_delete(list, deletable_idx);
}

void slowOptimizeList(List* list)
{
    Node* optimized_buffer = (Node*)calloc(list->size + 1, sizeof(Node));
    size_t cur_idx = list->head;
    for (size_t i = 0; i < list->size && referTo(cur_idx).next > 0; ++i)
    {
        optimized_buffer[i] = referTo(cur_idx);
        cur_idx = referTo(cur_idx).next;
    }
    memcpy(list->buffer + 1, optimized_buffer, sizeof(Node) * list->size);

    initNode(list, 0, 1, list->size, NAN); //<-- init fict Node
    for (size_t i = 1; i < list->size; ++i)
    {
        referTo(i).next = i + 1;
        referTo(i).prev = i - 1;
        referTo(i).status = OK;
    }
    initNode(list, list->size, 0, list->size - 1, referTo(list->size).value); //<-- init tail
    referTo(list->size).status = OK;

    list->free_head = list->size + 1;
    for (size_t free_idx = list->free_head; free_idx < list->capacity - 1; ++free_idx)
    {
        initNode(list, free_idx, -1, free_idx + 1, FREE);
    }
    initNode(list, list->capacity - 1, -1, 0, NAN);
    referTo(list->capacity - 1).status = FREE;

    free(optimized_buffer);

    list->head = 1;
    list->tail = list->size;

    list->is_optimized = true;
}   

void showListConsole(List* list, const char* reason)
{
    size_t cur_idx = list->head;
    printf("List: \n");
    showNode(list, list->head, "Head");
    showNode(list, list->tail, "Tail");
    size_t i = 0;
    for (size_t i = 0; i < list->size; ++i)
    {   
        printf("Logic number #%zu|", i + 1);
        showNode(list, cur_idx, reason);
        cur_idx = referTo(cur_idx).next;
    }
}

void showNode(List* list, size_t idx, const char* reason)
{
    printf("|%s|Next: %zu|Value: %lg|Prev: %zu|Virt address: %zu|\n", reason, referTo(idx).next, referTo(idx).value, referTo(idx).prev, idx);
}

void showBuffer(List* list)
{   
    printf("-------\n");
    printf("BUFFER:\n");
    for (size_t i = 0; i < list->capacity; ++i)
    {
        printf("Next: %zu|Value: %lg|Prev: %zu|Virt address: %zu\n", referTo(i).next, referTo(i).value, referTo(i).prev, i);
    }
}

void clearList(List* list)
{
    size_t capacity = list->capacity - 1;
    destructList(list);
    constructList(list, capacity - 1);
}

void destructList(List* list)
{
    free(list->buffer);
    printf("Oops\n");
}

List* deleteList(List* list)
{
    destructList(list);
    free(list);

    return NULL;
}

void determineNodeStatus_Assistent(List* list, size_t cur_idx)
{
    if (referTo(referTo(cur_idx).next).prev != cur_idx ||
        referTo(referTo(cur_idx).prev).next != cur_idx ||
        referTo(cur_idx).value == NAN)
    {
        referTo(cur_idx).status = BROKEN;
    }
    else  referTo(cur_idx).status = OK;
}

bool assertList(List* list)
{   
    if (list->capacity < 0 || list->size < 0 || list->size + 1 > list->capacity)
    {
        return false;
    }
    for (size_t i = 0; i < list->capacity; ++i)
    {
        if (referTo(i).status == BROKEN)
        {
            printf("Node with idx = %zu is died\n", i);
            return false;
        }
    }
    return true;
}

void logicListDump(List* list, const char* filename, const char* graph_filename)
{
    FILE* graph_file = fopen(filename, "w");

    // optimizeList(list);

    fprintf(graph_file, "digraph G{\n");
    fprintf(graph_file, "node [shape=\"record\", style=\"filled\", color=\"#008000\"];\n");

    fprintf(graph_file, "\"%p\" [color=\"#000080\", fillcolor=\"gray\", label=\"{FICT NODE}\"];\n", list->buffer + 0);

    size_t cur_idx = list->head;
    for (size_t i = 0; i < list->size && cur_idx > 0; ++i)
    {
        fprintf(graph_file, "\"%p\" [label=\"{value: %lg|{prev: %zu|cur: %zu|next: %zu}}\", ", &referTo(cur_idx), 
                referTo(cur_idx).value, referTo(cur_idx).prev, cur_idx, referTo(cur_idx).next); // <-- label printing
        
        fprintf(graph_file, "fillcolor=%s]\n", STATUS_COLORS[(int)referTo(cur_idx).status]); // <-- color printing

        fprintf(graph_file, "\"%p\"->\"%p\";\n", &referTo(cur_idx), &referTo(referTo(cur_idx).prev)); // <-- point to prev
        fprintf(graph_file, "\"%p\"->\"%p\";\n", &referTo(cur_idx), &referTo(referTo(cur_idx).next)); // <-- point to next
        cur_idx = referTo(cur_idx).next;
    }
    fprintf(graph_file, "}");

    fclose(graph_file);

    drawGraph(filename, graph_filename);
}

void realListDump(List* list, const char* filename, const char* graph_filename)
{
    FILE* graph_file = fopen(filename, "w");

    fprintf(graph_file, "digraph G{\n");
    fprintf(graph_file, "node [shape=\"record\", style=\"filled\", color=\"gray\"];\n");

    fprintf(graph_file, "\"%p\" [color=\"gray\", fillcolor=\"gray\", label=\"{FICT NODE}\"];\n", list->buffer + 0);

    //Just fucking setting my nu(o)des
    for (size_t cur_idx = 1; cur_idx < list->capacity - 1; ++cur_idx)
    {
        fprintf(graph_file, "\"%p\" [label=\"{val: %lg|{prev: %d|cur: %zu|next: %zu}}\", ", &referTo(cur_idx),           // <-- label printing
                referTo(cur_idx).value, referTo(cur_idx).prev, cur_idx, referTo(cur_idx).next); 
        fprintf(graph_file, "fillcolor=%s]\n", STATUS_COLORS[(int)referTo(cur_idx).status]);       // <-- color printing
    }
    //For last
    fprintf(graph_file, "\"%p\" [label=\"{val: %lg|{prev: %d|cur: %zu|next: %zu}}\", ", &referTo(list->capacity - 1),
            referTo(list->capacity - 1).value, referTo(list->capacity - 1).prev, list->capacity - 1, referTo(list->capacity - 1).next); 
    fprintf(graph_file, "fillcolor=%s]\n", STATUS_COLORS[(int)referTo(list->capacity - 1).status]);

    //Just fucking physical pointers
    for (size_t cur_idx = 0; cur_idx < list->capacity - 1; ++cur_idx)
    {
        fprintf(graph_file, "edge[color=red]\n\"%p\"->\"%p\";\n", &referTo(cur_idx), &referTo(cur_idx) + 1); 
    }
    fprintf(graph_file, "edge[color=red]\n\"%p\"->\"%p\";\n", &referTo(list->capacity - 1), list->buffer + 0); 


    //Just fucking logic pointers
    size_t i = 1;
    size_t cur_idx = list->head;
    fprintf(graph_file, "edge[color=black]\"%p\"->\"%p\"", list->buffer + 0, &referTo(list->head));
    while (i++ < list->size + 1)
    {
        if (cur_idx != list->tail)
        {
            fprintf(graph_file, "edge[color=black]\n\"%p\"->\"%p\";\n", &referTo(cur_idx), &referTo(referTo(cur_idx).next));
        }
        cur_idx = referTo(cur_idx).next;
    }

    fprintf(graph_file, "edge[color=black]\"%p\"->\"%p\"", &referTo(list->tail), list->buffer + 0);

    fprintf(graph_file, "}");

    fclose(graph_file);

    drawGraph(filename, graph_filename);
}

void drawGraph(const char* filename, const char* graph_filename)
{
    char* command = (char*)calloc(96, sizeof(char));

    size_t i = 0;
    while (DOT_COMMAND[i] != '\0')
    {
        command[i] = DOT_COMMAND[i];
        ++i;
    }
    size_t j = 0;
    while (filename[j] != '\0')
    {
        command[i] = filename[j++];
        ++i;
    }
    command[i++] = ' ';
    command[i++] = '>';
    command[i++] = ' ';
    j = 0;
    while (graph_filename[j] != '\0')
    {
        command[i] = graph_filename[j++];
        ++i;
    }
    
    system(command);

    free(command);
}

int pushUnitTests()
{
    List list = {};
    constructList(&list, 20);

    for (size_t i = 8; i > 0; --i)
    {
        pushForwardList(&list, i);
    }

    for (size_t i = 9; i < 15; ++i)
    {
        pushBackList(&list, i);
    }
    showListConsole(&list, "push");
    logicListDump(&list, "push_logic.txt", "push_logic_graph.jpg");
    realListDump(&list, "push_real.txt", "push_real_graph.jpg");

    size_t idx = list.head;
    for (size_t i = 1; i < 15; ++i)
    {
        if (list.buffer[idx].value != i)
        {   
            printf("[idx] = %lg, i = %zu\n", list.buffer[idx].value, i);
            printf("ERROR PUSH UNIT TEST\n");
            return 1;
        }
        idx = list.buffer[idx].next;
    }
    printf("push unit test passed\n");

    return 0;
}

int check(List* list, size_t correct_values[], size_t size, const char* reason)
{
    size_t idx = list->head;
    for (size_t i = 0; i < size; ++i)
    {
        if (list->buffer[idx].value != correct_values[i])
        {   
            printf("[idx] = %lg, correct_values[i] = %zu\n", list->buffer[idx].value, correct_values[i]);
            printf("ERROR %s UNIT TEST\n", reason);
            return 1;
        }
        idx = list->buffer[idx].next;
    }

    return 0;
}

int insertUnitTest()
{
    List list = {};
    constructList(&list, 30);

    size_t correct_values[] = {1, 2, 3, 4, 400, 401, 402, 5};

    for (size_t i = 0; i < 5; ++i)
    {
        pushBackList(&list, i + 1);
    }
    size_t cur_idx = getIndexByNumber(&list, 4);
    for (size_t i = 0; i < 3; ++i)
    {
        cur_idx = realInsertAfter(&list, cur_idx, 400 + i);
    }
    showListConsole(&list, "insert");
    logicListDump(&list, "insert_logic.txt", "insert_logic_graph.jpg");
    realListDump(&list, "insert_real.txt", "insert_real_graph.jpg");

    if (check(&list, correct_values, 8, "insert") != 0)
    {
        return 1;
    }

    printf("insert unit test passed\n");  
    return 0;
}

int deleteUnitTests()
{
    List list = {};
    constructList(&list, 5);
    size_t correct_values[] = {1, 2, 4, 5};

    for (size_t i = 1; i <= 5; ++i)
    {
        pushBackList(&list, i);
    }
    size_t idx = getIndexByNumber(&list, 3);
    service_delete(&list, idx);

    showListConsole(&list, "delete");
    logicListDump(&list, "delete_logic.txt", "delete_logic_graph.jpg");
    realListDump(&list, "delete_real.txt", "delete_real_graph.jpg");
    
    if (check(&list, correct_values, 4, "delete") != 0)
    {
        return 1;
    }

    printf("delete unit tests passed\n");
    return 0;
}

void unitTests()
{
    pushUnitTests();
    insertUnitTest();
    deleteUnitTests();
}