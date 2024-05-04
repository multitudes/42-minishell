#include <stdio.h>
#include  <stdlib.h>
#include <string.h>

/*
this is due to an article about how linux kernel uses linked lists
https://kernelnewbies.org/FAQ/LinkedLists

https://github.com/mkirchner/linked-list-good-taste

*/

struct list_item {
        int value;
        struct list_item *next;
};
typedef struct list_item list_item;

struct llist {
        struct list_item *head;
};
typedef struct llist llist;
/* The textbook version */
void remove_cs101(llist *l, list_item *target);
/* A more elegant solution by linus*/
void remove_elegant(llist *l, list_item *target);

void remove_cs101(llist *l, list_item *target)
{
        list_item *cur = l->head, *prev = NULL;
        while (cur != target) {
                prev = cur;
                cur = cur->next;
        }
        if (prev)
                prev->next = cur->next;
        else
                l->head = cur->next;
}

void remove_elegant(llist *l, list_item *target)
{
        list_item **p = &l->head;
        while (*p != target)
                p = &(*p)->next;
        *p = target->next;
}

int main()
{
	list_item *item1 = malloc(sizeof(list_item));
	item1->value = 12;
	list_item *item2 = malloc(sizeof(list_item));
	item2->value = 99;
	item1->next = item2;
	list_item *item3 = malloc(sizeof(list_item));
	item3->value = 37;
	item2->next = item3;
	list_item *item4 = malloc(sizeof(list_item));
	item4->value = 42;
	item3->next = item4;
	llist *my_list = malloc(sizeof(llist));
	my_list->head = item1;

	remove_cs101(my_list, item2);
	printf("After remove_cs101: %d\n", my_list->head->next->value);
	remove_elegant(my_list, item3);
	printf("After remove_elegant: %d\n", my_list->head->next->value);
    return 0;
    }