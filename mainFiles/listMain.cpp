#include "headers/listHeader.h"


int main()
{   

    List list = {};
    constructList(&list, 30);
    int random = 0;

    for (size_t i = 0; i < 10; ++i)
    {
        pushForwardList(&list, 10 + i);
    }
    for (size_t i = 0; i < 7; ++i)
    {
        pushBackList(&list, 20 + i);
    }
    for (size_t i = 0; i < 10; ++i)
    {
        random = abs(rand() % 14 + 3);
        insertAfter(&list, random, 17 + i);
    }
    for (size_t i = 0; i < 5; ++i)
    {   
        random = abs(rand() % 14 + 2);
        deleteNode(&list, random);
    }

    showListConsole(&list, "look");

    realListDump(&list, "real_del_ins.txt", "real_del_ins_graph.jpg");
    logicListDump(&list, "logic_del_ins.txt", "logic_del_ins_graph.jpg");

    // list.buffer[list.buffer[list.head].next].status = BROKEN;
    
    // realListDump(&list, "re_one_red.txt", "re_one_red.jpg");
    // logicListDump(&list, "lo_one_red.txt", "lo_one_red.jpg");

    slowOptimizeList(&list);

    realListDump(&list, "re_opt.txt", "re_opt_gra.jpg");
    logicListDump(&list, "lo_opt.txt", "lo_opt_gra.jpg");


    
    return 0;
}

