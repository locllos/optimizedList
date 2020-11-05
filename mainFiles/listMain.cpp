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
        // printf("Random: %d\n", random);
        insertAfter(&list, random, 17 + i);
    }
    for (size_t i = 0; i < 5; ++i)
    {   
        random = abs(rand() % 14 + 2);
        // printf("Random: %d\n", random);
        deleteNode(&list, random);
    }

    showListConsole(&list, "look");

    realListDump(&list, "real_del_ins.txt", "real_del_ins_graph.jpg");
    logicListDump(&list, "logic_del_ins.txt", "logic_del_ins_graph.jpg");

    // list.buffer[list.buffer[list.head].next].status = BROKEN;
    
    // realListDump(&list, "re_one_red.txt", "re_one_red.jpg");
    // logicListDump(&list, "lo_one_red.txt", "lo_one_red.jpg");
    
    return 0;
}

// void insertAfter_Example_logic()
// {
//     List list = {};
//     constructList(&list, 50);
//     //showBuffer(&list);
//     for (size_t i = 0; i < 10; ++i)
//     {
//         pushBackList(&list, i + 100);
//     }
//     for (size_t i = 0; i < 10; ++i)
//     {
//         insertAfter(&list,  6, 1337 + i);
//     }
//     showListConsole(&list, "loop?");
//     logicListDump(&list);
// }


    // List list = {};
    // constructList(&list, 50);
    // //showBuffer(&list);
    // for (size_t i = 0; i < 10; ++i)
    // {
    //     pushBackList(&list, i + 100);
    // }
    // for (size_t i = 0; i < 4; ++i)
    // {
    //     deleteNode(&list, i % 3 + 2);
    // }
    // showListConsole(&list, "loop?");
    // logicListDump(&list, "check_delete.txt", "logic_graph_when_delete.jpg");
    
    // return 0;

    //     List list = {};
    // constructList(&list, 50);
    // //showBuffer(&list);
    // for (size_t i = 0; i < 10; ++i)
    // {
    //     pushBackList(&list, i + 100);
    // }
    // for (size_t i = 0; i < 4; ++i)
    // {
    //     deleteNode(&list, i % 3 + 2);
    // }
    // showListConsole(&list, "loop?");
    // logicListDump(&list, "check_delete.txt", "check.jpg");