#include "headers/listHeader.h"

const char* DOT_COMMAND = "dot -Tjpg ";

const char* STATUS_COLORS[] = 
{
    "yellow",
    "white",
    "red",
    "green"
};

void Copy(void* value_to, const void* value_from, const size_t elem_size) 
{
    for (size_t i = 0; i < elem_size; ++i) 
        *((uint8_t*)value_to + i) = *((uint8_t*)value_from + i);
}

void initNode(List* list, virtual_ptr node)
{
    referTo(node).next = 0;
    referTo(node).prev = 0;
    referTo(node).value = NAN;
    referTo(node).status = NEW;
}

void initNode(List* list, virtual_ptr node, elem_t value)
{
    referTo(node).next = 0;
    referTo(node).prev = 0;
    referTo(node).value = value;
    referTo(node).status = NEW;
}

void initNode(List* list, virtual_ptr node, virtual_ptr prev, virtual_ptr next, elem_t value)
{
    referTo(node).prev = prev;
    referTo(node).next = next;
    referTo(node).value = value;
    referTo(node).status = NEW;
}

void constructList(List* list, size_t start_capacity)
{
    list->size = 0;
    list->capacity = start_capacity + 1;
    list->free = 1;
    list->is_optimized = false;
    list->head = 1;
    list->tail = 1;

    list->buffer = (Node*)calloc(list->capacity, sizeof(Node));
    initNode(list, 0, 0, 0, NAN);
    referTo(0).status = OK;

    virtual_ptr cur_node = 1;
    for (; cur_node < list->capacity; ++cur_node)
    {
        initNode(list, cur_node, -1, cur_node + 1, NAN);
        referTo(cur_node).status = FREE;
    }
    initNode(list, cur_node - 1, -1, 0, NAN);
    referTo(cur_node - 1).status = FREE;
}

List* newList(size_t start_capacity)
{
    List* list = (List*)calloc(1, sizeof(List));

    constructList(list, start_capacity);

    return list;
}

void freeMemory(List* list, virtual_ptr deletable_node)
{
    initNode(list, deletable_node, -1, list->free, NAN);

    referTo(deletable_node).status = FREE;

    list->free = deletable_node;
}

virtual_ptr getFirst(List* list)
{
    return list->head;
}

virtual_ptr getLast(List* list)
{
    return list->tail;
}

virtual_ptr getFreeSpace(List* list)
{   
    assert(list->size + 1 < list->capacity);

    virtual_ptr free = list->free;

    list->free = list->buffer[free].next;

    return free;
}

virtual_ptr getVirtualAddressByPos(List* list, size_t pos)
{
    //Можно применить "оптимизацию", если pos > size / 2, то пробегаться с конца
    if (list->is_optimized) return pos;

    virtual_ptr cur_node = list->head;
    for (size_t i = 1; i < pos; ++i)
    {
        cur_node = referTo(cur_node).next;
    }
    return cur_node;
}

virtual_ptr insertBetween(List* list, virtual_ptr node_before, virtual_ptr node_after, elem_t value)
{   
    LIST_ASSERT(list);
    assert(node_before >= 0);
    assert(node_before < list->capacity);
    assert(node_after >= 0);
    assert(node_after < list->capacity);

    virtual_ptr insertion_node = getFreeSpace(list);

    initNode(list, insertion_node, node_before, node_after, value);
    referTo(node_before).next = insertion_node;
    referTo(node_after).prev = insertion_node;

    ++list->size;

    LIST_ASSERT(list);

    determineNodeStatus_Assistent(list, insertion_node);

    return insertion_node;
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

void insertAfter(List* list, size_t pos_after, elem_t value)
{   
    // printf("i am here. pos = %zu\n", pos_after);
    virtual_ptr node_after = getVirtualAddressByPos(list, pos_after);

    insertBetween(list, node_after, referTo(node_after).next, value);
}

void insertBefore(List* list, size_t pos_before, elem_t value)
{
    virtual_ptr node_before = list->is_optimized ? pos_before : getVirtualAddressByPos(list, pos_before);
    
    insertBetween(list, referTo(node_before).prev, node_before, value);
}

void _service_delete_(List* list, virtual_ptr deletable_node)
{
    LIST_ASSERT(list);
    assert(deletable_node > 0);
    assert(deletable_node < list->capacity);

    referTo(referTo(deletable_node).next).prev = referTo(deletable_node).prev;
    referTo(referTo(deletable_node).prev).next = referTo(deletable_node).next;

    freeMemory(list, deletable_node);

    --list->size;

    LIST_ASSERT(list);
}

void deleteNode(List* list, size_t pos)
{   
    assert(pos < list->size + 1);

    virtual_ptr deletable_node = getVirtualAddressByPos(list, pos);

    if (deletable_node == list->head)
    {
        list->head = referTo(deletable_node).next;
    }
    else if (deletable_node == list->tail)
    {
        list->tail = referTo(deletable_node).prev;
    }
    _service_delete_(list, deletable_node);
}

void deleteHead(List* list)
{
    virtual_ptr deletable_node = list->head;

    list->head = referTo(deletable_node).next;

    _service_delete_(list, deletable_node);
}

void deleteTail(List* list)
{
    virtual_ptr deletable_node = list->tail;

    list->tail = referTo(deletable_node).prev;

    _service_delete_(list, deletable_node);
}

void optimizeList(List* list)
{
	Node* optimized_buffer = (Node*)calloc(list->size + 1, sizeof(Node));
    virtual_ptr cur_node = list->head;
	for (size_t i = 0; i < list->size && referTo(cur_node).next > 0; ++i)
    {
        optimized_buffer[i] = referTo(cur_node);
    }
    for (size_t pos = 0; pos < list->size; ++pos)
    {
        list->buffer[pos + 1] = optimized_buffer[pos];
    }

    list->free = list->size + 1;
    for (size_t free_pos = list->free; free_pos < list->capacity - 1; ++free_pos)
    {
        initNode(list, free_pos, -1, free_pos + 1, NAN);
        referTo(free_pos).status = FREE;
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
    virtual_ptr cur_node = list->head;
    Node node = list->buffer[cur_node];
    printf("List: \n");
    showNode(list, list->head, "Head");
    showNode(list, list->tail, "Tail");
    size_t i = 0;
    for (size_t i = 0; i < list->size; ++i)
    {   
        printf("Logic number #%zu|", i + 1);
        showNode(list, cur_node, reason);
        cur_node = referTo(cur_node).next;
    }
}

void showNode(List* list, virtual_ptr node, const char* reason)
{
    printf("|%s|Next: %zu|Value: %lg|Prev: %zu|Virt address: %zu|\n", reason, referTo(node).next, referTo(node).value, referTo(node).prev, node);
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

void determineNodeStatus_Assistent(List* list, virtual_ptr cur_node)
{
    if (referTo(referTo(cur_node).next).prev != cur_node ||
        referTo(referTo(cur_node).prev).next != cur_node ||
        referTo(cur_node).value == NAN)
    {
        referTo(cur_node).status = BROKEN;
    }
    else  referTo(cur_node).status = OK;
}

void determineNodesStatuses_Cheif_Please(List* list)
{
    virtual_ptr cur_node = list->head;

    if (referTo(referTo(cur_node).next).prev != cur_node || referTo(cur_node).value == NAN)
    {
        referTo(cur_node).status = BROKEN;
    }
    else  referTo(cur_node).status = OK;

    for (size_t i = 0; i < list->size && cur_node != list->tail; ++i)
    {
        determineNodeStatus_Assistent(list, cur_node);
    }
    cur_node = list->tail;
    if (referTo(referTo(cur_node).prev).next != cur_node || referTo(cur_node).value == NAN)
    {
        referTo(cur_node).status = BROKEN;
    }
    else  referTo(cur_node).status = OK;
}

bool assertList(List* list)
{   
    //Должна смотреть в узлах только на поля status, написать отдельную ф-ю, определяющую статус ноды
    if (list->capacity < 0 || list->size < 0 || list->size + 1 > list->capacity)
    {
        return false;
    }
    for (size_t i = 0; i < list->capacity; ++i)
    {
        if (referTo(i).next < 0 || referTo(i).prev < 0)
        {
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

    virtual_ptr cur_node = list->head;
    for (size_t i = 0; i < list->size && cur_node > 0; ++i)
    {
        fprintf(graph_file, "\"%p\" [label=\"{%lg|{%zu|%zu|%zu}}\", ", &referTo(cur_node), 
                referTo(cur_node).value, referTo(cur_node).prev, cur_node, referTo(cur_node).next); // <-- label printing
        
        fprintf(graph_file, "fillcolor=%s]\n", STATUS_COLORS[(int)referTo(cur_node).status]); // <-- color printing

        fprintf(graph_file, "\"%p\"->\"%p\";\n", &referTo(cur_node), &referTo(referTo(cur_node).prev)); // <-- point to prev
        fprintf(graph_file, "\"%p\"->\"%p\";\n", &referTo(cur_node), &referTo(referTo(cur_node).next)); // <-- point to next
        cur_node = referTo(cur_node).next;
    }
    fprintf(graph_file, "}");

    fclose(graph_file);

    drawGraph(filename, graph_filename);
}

void realListDump(List* list, const char* filename, const char* graph_filename)
{
    FILE* graph_file = fopen(filename, "w");

    fprintf(graph_file, "digraph G{\n");
    fprintf(graph_file, "node [shape=\"record\", style=\"filled\", color=\"#008000\"];\n");

    for (virtual_ptr cur_node = 1; cur_node < list->capacity; ++cur_node)
    {
        fprintf(graph_file, "\"%p\" [color=\"%s\", label=\"{%lg|{%d|%zu|%zu}}\"];\n", &referTo(cur_node),
        STATUS_COLORS[(int)referTo(cur_node).status], referTo(cur_node).value, referTo(cur_node).prev, cur_node, 
        referTo(cur_node).next);
        fprintf(graph_file, "edge[color=black]\n\"%p\"->\"%p\";\n", &referTo(cur_node), &referTo(cur_node + 1));
    }

    fprintf(graph_file, "\"HEAD\" [color=\"#000080\", fillcolor=\"gray\", label=\"{FICT NODE}\"];\n");
    fprintf(graph_file, "edge[color=black]\n\"FICT NODE\"->\"%p\"", &referTo(list->head));

    virtual_ptr cur_node = list->head;
    size_t i = 1;
    while (cur_node != list->tail && i < list->size + 1)
    {
        fprintf(graph_file, "edge[color=black]\n\"%p\"->\"%p\";\n", &referTo(cur_node), &referTo(referTo(cur_node).next));
        cur_node = referTo(cur_node).next;
    }

    fprintf(graph_file, "\"%p\"->\"HEAD\"", &referTo(list->tail));

    fprintf(graph_file, "}");

    fclose(graph_file);

    drawGraph(filename, graph_filename);
}


void drawGraph(const char* filename, const char* graph_filename)
{
    char* command = (char*)calloc(128, sizeof(char));
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

