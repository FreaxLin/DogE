#ifndef _METHOD_AREA_H
#define _METHOD_AREA_H
#include "./extern_dependency/hashtable.h"
#include "./extern_dependency/arraylist.h"
#include <stdlib.h>
#endif

typedef struct{
    char* class_name;
    int size;
    hashtable* field_table;
    hashtable* def_table;
    int field_count;
    int def_count;
    char** field_array;
    char** def_array;
}class_meta;

typedef struct{
    char* field_name;
    char type;
    int offset;
    void* value;
}field_meta;

typedef struct{
    char* def_name;
    char* return_type;
    int size;
    int nums;
    int command_count;
    char** command_array;
}def_meta;

field_meta* field_meta_new();

void command_array_expansion(char** command_array);

def_meta* get_def(class_meta* cm, char* def_name);