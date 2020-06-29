#ifndef _MINI_VM_H
#define _MINI_VM_H
#include "stack.h"
#include "heap.h"
#endif

void bootstrap(class_meta* cm, char* def);
int run_command(class_meta* cm, stack_frame* sf, def_meta* def, int command_index);
int run_set(stack_frame* sf, int index, void* value);
void run_print(stack_frame* sf, int index);
void run_small_than(stack_frame* sf, int index, int* value);
int run_jump(stack_frame* sf, int pre, int next);
void* new_object(class_meta* cm, char* class_name);