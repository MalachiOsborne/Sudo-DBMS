#ifndef DATABASE_MANAGEMENT_SYSTEM_FUNCTIONS
#define DATABASE_MANAGEMENT_SYSTEM_FUNCTIONS
#include "llf.h"
#include "windows_portability.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int ids[256];
extern int ids_counter;
extern char* headers[MAX_HEADERS];
extern node* headofheads;
extern bool loaded;

void present_options(void);
void search_options(void);
void insert_options(const char* csv);
void update_options(const char* csv);
void delete_options(const char* csv);
void usage(void);
bool load(const char* csv);
bool unload(void);
bool update_csv(const char* csv, node* head);
#endif
