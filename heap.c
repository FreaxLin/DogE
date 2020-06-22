#include <stdio.h>
#include <stdlib.h>

typedef struct _SPACE_POINTER{
    int size;
    struct _SPACE_POINTER* next;
    struct _SPACE_POINTER* pre;
    void* free_address;
}free_space_pointer;

typedef struct{
    int label;
    int size;
}object_header;

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
            temp->free_address = temp->free_address + size;
            return result;
        }
        temp = temp->next;
    }
    return result;
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



int main(int argc, char** argv){
    allocate_heap_space(10);
}