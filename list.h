
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct LINKEDLIST *LinkedList;
typedef struct NODE *Node;
struct LINKEDLIST {
    Node top;
    int size;
};
struct NODE {
    Node next;
    int data;
};


Node create_node(int data, Node next);

LinkedList create_linked_list();

bool add_item(LinkedList list, int data);

bool remove_item(LinkedList list, int data);

void print_items(LinkedList list);


#endif