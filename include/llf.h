#ifndef LINKED_LIST_FUNCTION_HEADER
#define LINKED_LIST_FUNCTION_HEADER
#include "windows_portability.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEADERS 3

typedef struct node
{
    int id;
    char* name;
    char* age;
    struct node* next;
}
node;
extern int ids[256];
extern int ids_counter;
extern char* headers[MAX_HEADERS];
extern node* headofheads;
extern bool loaded;
//hi
void display_list(node* head);
void display_display_list(void);
void bubble_sort(int a[], int size);
node* create_node(int id[], char name[], char age[],int id_length);
node* append_node(node* head, int id[], char name[], char age[], int id_length);
node* insert_after_x(node* head, int key, int id[], char name[], char age[], int id_length);
node* delete_first_node(node* head);
node* delete_node(node* head, int key);
bool display_entry_by_id(node* head, int key);
int display_entry_by_name(node* head, char* name, int entries_ids[]);
int display_entry_by_age(node* head, char* age, int entries_ids[]);
int generate_id(void);
node* update_id(node* head, int old_id, int new_id);
node* update_name_by_id(node* head, char* new_name, int name_id);
node* update_age_by_id(node* head, char* new_age, int age_id);
#endif
