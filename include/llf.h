#ifndef LINKED_LIST_FUNCTION_HEADER
#define LINKED_LIST_FUNCTION_HEADER
#include "llf.h"
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
//hi
void present_options(void);
void search_options(void);
void insert_options(const char* csv);
void update_options(const char* csv);
void delete_options(const char* csv);
void usage(void);
void display_list(node* head);
void display_display_list(void);
void bubble_sort(int a[], int size);
bool load(const char* csv);
bool unload(void);
bool update_csv(const char* csv, node* head);
bool display_entry_by_id(node* head, int key);
int display_entry_by_name(node* head, char* name, int entries_ids[]);
node* create_node(int id[], char name[], char age[],int id_length);
node* append_node(node* head, int id[], char name[], char age[], int id_length);
node* insert_after_x(node* head, int key, int id[], char name[], char age[], int id_length);
node* delete_node(node* head, int key);
#endif
