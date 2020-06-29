#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "method_area.h"
#include "miniVM.h"



void bootstrap(class_meta* cm, char* def){

    

    printf("开始执行%s方法\n", def);
    def_meta* start = get_def(cm, def);

    int local_var_num = start->nums;
    int local_var_size = start->size;
    int operate_stack_size = 64;
    int* local_var_offset = start->off_set;
    stack_frame sf = init_stack_frame(operate_stack_size, local_var_num, local_var_size, local_var_offset);
    int command_count = start->command_count;
    int command_index = 0;
    while (command_index != -1){
        command_index = run_command(cm, &sf, start, command_index);
    }
    
}

int run_command(class_meta* cm, stack_frame* sf, def_meta* def, int command_index){
    char* command = def->command_array[command_index];
    char* temp = strtok(command, "#");
    int local_var_index = -1;
    int int_value;
    char* char_value;
    char* command_header = temp;

    if (strstr(command_header, "set") || strstr(command_header, "push") || strstr(command_header, "print") 
        || strstr(command_header, "st")){
        char* pos = strchr(temp, '_');
        if (pos != NULL){
            local_var_index = atoi(pos);
        }
        temp = strtok(NULL, "#");
        if (temp != NULL){
            int_value = atoi(temp);
        }
        if (strstr(command_header, "set")){
            run_set(sf, local_var_index, &int_value);
        }
        if (strstr(command_header, "push")){
            run_set(sf, local_var_index, &int_value);
        }
        if (strstr(command_header, "print")){
            run_print(sf, local_var_index);
        }
        if (strstr(command_header, "st")){
            run_small_than(sf, local_var_index, &int_value);
        }
    }
    if (strstr(command_header, "jump")){
        temp = strtok(NULL, "#");
        int pre = atoi(temp);
        temp = strtok(NULL, "#");
        int next = -1;
        if (temp != NULL){
            next = atoi(temp);
        }
        return run_jump(sf, pre, next);
    }

    if (strstr(command_header, "init")){
        temp = strtok(NULL, "#");
    }
    
    command_index++;
    if (command_index < def->command_count){
        return command_index;
    }else{
        return -1;
    }
}

int run_set(stack_frame* sf, int index, void* value){
    set_local_var_value(&(sf->local_var_array), index, value, sizeof(int));
    return 0;
}

void run_print(stack_frame* sf, int index){
    void* value = get_local_var_value(&(sf->local_var_array), index);
    printf("程序输出:%d", *(int*)value);
}

void run_small_than(stack_frame* sf, int index, int* value){
    int* var_value = (int*)get_local_var_value(&(sf->local_var_array), index);
    if (*var_value < *value){
        push_operate_stack_int(&(sf->os), 1);
    }else{
        push_operate_stack_int(&(sf->os), 0);
    }
}

int run_jump(stack_frame* sf, int pre, int next){
    if (next == -1){
        return pre;
    }
    int i = pop_operate_stack_int(&(sf->os));
    if (i > 0){
        return pre;
    }else{
        return next;
    }
}


