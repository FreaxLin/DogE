#include <stdio.h>
#include "method_area.h"

static hashtable* class_meta_table;

void method_area_new(){
    hashtable* class_meta_table = hashtable_create();
}

void* get_class_info(char* class_name){
    return hashtable_get(class_meta_table, class_name);
}


field_meta* field_meta_new(){
    field_meta* fm = (field_meta*) malloc(sizeof(field_meta));
    return fm;
}

void field_meta_delete(field_meta* fm){
    if (fm->field_name != NULL){
        free(fm->field_name);
    }
    if (fm->value != NULL){
        free(fm->value);
    }
    free(fm);
}
