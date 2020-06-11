#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct{

}class_meta;

typedef struct{
    class_meta* pointer;
    int size;
}OBJECT_HEADER;

OBJECT_HEADER* object_malloc(class_meta* meta, int value_size){
    OBJECT_HEADER* header = (OBJECT_HEADER*)malloc(sizeof(OBJECT_HEADER) + value_size);
    header->pointer = meta;
    header->size = value_size;
    return header;
}

void command_run(){

}

int main(int argc, char **argv){
    OBJECT_HEADER* header = (OBJECT_HEADER*)malloc(sizeof(OBJECT_HEADER) + 9);
    int* value1 = (int*)(header + sizeof(OBJECT_HEADER));
    *value1 = 10;
    char* value2 = (char*)(value1 + 4);
    *value2 = 'c';
    int* value3 = (int*)(value2 + 1);
    *value3 = 11;
    printf("%d\n", *(int*)(header + sizeof(OBJECT_HEADER)));
    printf("%c\n", *(char*)(header + sizeof(OBJECT_HEADER) + 1));
    
    return 0;
}