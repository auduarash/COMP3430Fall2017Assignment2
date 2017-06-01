
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct LINKEDLIST *LinkedList;
typedef struct NODE *Node;
typedef struct LOG *Log;
struct LINKEDLIST {
    Node top;
    int size;
};
struct NODE {
    Node next;
    Log data;
};
struct LOG {
    int row;
    int x_location; // current x location of the log
    int y_location; // the current y location of the log
    int direction; //is the log heading east or west
    int frequency; //how fast is the log moving per frame
    bool player_on_log; // is the player currently on this log
    int animation; //four animations exist for the front and end of the log
};


Node create_node(Log data, Node next);

LinkedList create_linked_list();

bool add_item(LinkedList list, Log data);

bool remove_item(LinkedList list, Log data);

void print_items(LinkedList list);


#endif