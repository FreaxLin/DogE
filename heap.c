#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void* find_free_space(free_space_pointer* header, int size){
    free_space_pointer* temp = header;
    void* result = NULL;
    while (temp != NULL){
        if (temp->size == size){
            result = temp->free_address;
            if (temp == header){
                header = header->next;
            }else{
                temp->pre->next = temp->next;
                temp->next->pre = temp->pre;
            }
            free(temp);
            return result;
        }
        if (temp->size > size){
            result = temp->free_address;
            temp->size = temp->size - size;
            temp->free_address = temp->free_address + size;
            return result;
        }
        temp = temp->next;
    }
    return result;
}

void gc(void* heap_header, void** objects, int count){
    for (int i = 0; i < count; i++){
        make(objects[i]);
    }
    
}

void sweep(void* heap_header){

    

}
void mark(void* object){
    //标志位为1;
    *(int*)object = 1;
}

void add_free_space(free_space_pointer* header, void* space_address, int size){
    free_space_pointer* pointer = (free_space_pointer*) malloc(sizeof(free_space_pointer));
    pointer->next = header->next;
    pointer->pre = header;
    header->next->pre = pointer;
    header->next = pointer;
    pointer->size = size;
}

free_space_pointer* allocate_heap_space(int size){
    void* pointer = malloc(size);
    free_space_pointer* header = (free_space_pointer*) malloc(sizeof(free_space_pointer));
    header->size = size;
    header->free_address = pointer;
    header->next = NULL;
    header->pre = NULL;
    return header;
}

