#ifndef _STACK_H
#define _STACK_H
#endif

typedef struct {
    int* offset;
    void* local_var;
}stack_slot;

typedef struct{
    int size;
    int count;
    void* header;
    void* stack;
}operate_stack;

typedef struct {
    stack_slot local_var_array;
    operate_stack os;
}stack_frame;

stack_slot init_local_var(int num, int size, int* index);
void destory_local_var(stack_slot* slot);
void* get_local_var_value(stack_slot* slot, int index, void* value);
void set_local_var_value(stack_slot* slot, int index, void* value);
operate_stack init_operate_stack(int size);
void destory_operate_stack(operate_stack* os);
stack_frame init_stack_frame(int operate_stack_size, int local_var_num, int local_var_size, int* index);