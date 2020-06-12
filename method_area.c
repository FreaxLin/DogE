#include <stdio.h>
#include "hashtable.h"
#include "arraylist.h"

typedef struct{
    char* class_name;
    hashtable* field_table;
    hashtable* def_table;
}class_meta;

typedef struct{
    char* field_name;
    char* type;
    int offset;
}field_meta;

typedef struct{
    char* def_name;
    char* return_type;
    int size;
    int nums;
    arraylist* command_array;
}def_meta;

static hashtable* class_meta_table;

void method_area_new(){
    hashtable* class_meta_table = hashtable_create();
}

void* get_class_info(char* class_name){
    return hashtable_get(class_meta_table, class_name);
}

int main(int argc, char **argv){
    
}