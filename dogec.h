#ifndef _DOGEC_H
#define _DOGEC_H
#include "./extern_dependency/mpc.h"
#include "method_area.h"
#endif 

typedef struct class_field{
    char* name;
    char* type;
    void* value;
    struct class_field *next;
}CLASS_FIELD;

typedef struct{
    
}FUNCTION_STMT;

typedef struct {
    char* name;
    char* return_type;

}CLASS_FUNCTION;

typedef struct {
    char* name;
    int field_num;
    CLASS_FIELD fields[1]; 
    int function_num;
    CLASS_FUNCTION functions[];
}CLASS_INFO;

void init_compile();
void destory_compile();
int compile_decls(mpc_ast_t* t, class_meta* cm, hashtable* ft);
void compile_procedure(mpc_ast_t* t, class_meta* cm, hashtable* ft);
class_meta compile_class(mpc_ast_t* t);
class_meta compile_doge(char* file);