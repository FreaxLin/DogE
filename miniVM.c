#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "stack.h"
#include "method_area.h"
#include "miniVM.h"


void bootstrap(class_meta* cm, char* def){
    def_meta* start = get_def(cm, def);

    int local_var_num = start->nums;
    int local_var_size = start->size;
    int operate_stack_size = 64;
}

int parse_command(char* command){
    return 0;
}