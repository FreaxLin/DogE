#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "method_area.h"
#include "miniVM.h"

static int heap_size = 100;
static free_space_pointer* fsp;
static void* heap_header;

void bootstrap(class_meta* cm, char* def){

    fsp = allocate_heap_space(heap_size);
    heap_header = fsp->free_address;
    printf("初始化堆%dB空间\n", heap_size);

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
    char temp_command[strlen(command) + 1];
    strcpy(temp_command, command);
    char* temp = strtok(temp_command, "#");
    int local_var_index = -1;
    int int_value;
    int* value_pointer = NULL;
    char* char_value;
    char* command_header = temp;

    if (strstr(command_header, "set") || strstr(command_header, "push") || strstr(command_header, "print") 
        || strstr(command_header, "st") || strstr(command_header, "objectset")){
        char* pos = strchr(temp, '_');
        if (pos != NULL){
            local_var_index = atoi(pos+1);
        }
        temp = strtok(NULL, "#");
        if (temp != NULL){
            int_value = atoi(temp);
            value_pointer = &int_value;
        }
        if (strstr(command_header, "set")){
            run_int_set(sf, local_var_index, value_pointer);
        }
        if (strstr(command_header, "push")){
            run_push(sf, local_var_index, value_pointer);
        }
        if (strstr(command_header, "print")){
            run_print(sf, local_var_index);
        }
        if (strstr(command_header, "st")){
            run_small_than(sf, local_var_index, value_pointer);
        }
        if (strstr(command_header, "objectset")){
            run_object_set(sf, local_var_index);
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
        void* object = new_object(cm, temp);
        push_operate_stack_object(&(sf->os), object);
    }

    if (strstr(command_header, "add")){
        run_add(sf,0);
    }
    
    
    command_index++;
    if (command_index < def->command_count){
        return command_index;
    }else{
        return -1;
    }
}

int run_int_set(stack_frame* sf, int index, void* value){
    if (value != NULL){
        set_local_var_value(&(sf->local_var_array), index, value, sizeof(int));
    }else{
        int i = pop_operate_stack_int(&(sf->os));
        set_local_var_value(&(sf->local_var_array), index, &i, sizeof(int));
    }
    return 0;
}

void run_object_set(stack_frame* sf, int index){
    void* object = pop_operate_stack_object(&(sf->os));
    set_local_var_value(&(sf->local_var_array), index, object, sizeof(void*));

}

void run_push(stack_frame* sf, int index, void* value){
    void* var = get_local_var_value(&(sf->local_var_array), index);
    push_operate_stack_int(&(sf->os), *(int*)var);
}

void run_add(stack_frame* sf, int index){
    int first = pop_operate_stack_int(&(sf->os));
    int second = pop_operate_stack_int(&(sf->os));
    int result = first + second;
    push_operate_stack_int(&(sf->os), result);
}

void run_print(stack_frame* sf, int index){
    void* value = get_local_var_value(&(sf->local_var_array), index);
    printf("程序输出:%d\n", *(int*)value);
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

void* new_object(class_meta* cm, char* class_name){
    int header_len = sizeof(object_header);
    int size = header_len + cm->size;
    void* object = find_free_space(fsp, size);
    if (object == NULL){
        printf("堆空间不足,启动gc\n");
    }
    //标志位为0;
    *(int*)object = 0;
    *(int*)(object + 4) = cm->size;

    //初始化对象
    for (int i = 0; i < cm->field_count; i++){
        char* field = cm->field_array[i];
        field_meta* fm = get_filed(cm, field);
        if (fm->value != NULL){
            int pos = fm->offset + header_len;
            *(int*)(object + pos) = *(int*)fm->value;
        }
    }
    return object;
}


