#include "headers/listHeader.h"


int main()
{   

    List list = {};
    constructList(&list, 50);
    int random = 0;

    for (size_t i = 0; i < 20; ++i)
    {
        pushForwardList(&list, 10 + i);
    }
    for (size_t i = 0; i < 5; ++i)
    {   
        random = abs(rand() % 14 + 2);
        printf("Random: %d\n", random);
        deleteNode(&list, random);
    }
    for (size_t i = 0; i < 7; ++i)
    {
        insertAfter(&list, i % 5 + 2, 1000 * (i + 1));
    }
    showListConsole(&list, "look");

    ListDumpReal(&list);
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