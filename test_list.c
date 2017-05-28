#include <stdio.h>
#include <stdlib.h>
#include "list.h"



int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s <number1> <number2> ... <numbern> \n", argv[0]);
    }

    LinkedList my_list = create_linked_list();
    int i;
    for (i = 1; i < argc; i++){
        int x = atoi(argv[i]);
        add_item(my_list, x);
        
    }
    print_items(my_list);

    for (i = 1; i < argc - 2; i++) {
        int x = atoi(argv[i]);
        remove_item(my_list, x);
    }

    print_items(my_list);
    return 0;
}