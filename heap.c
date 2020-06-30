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

free_space_pointer* gc(void* heap_header, void** objects, int count, int heap_size, free_space_pointer* sfp){
    for (int i = 0; i < count; i++){
        mark_object(objects[i]);
    }
    return sweep(heap_header, heap_size, sfp);
    
}

free_space_pointer* sweep(void* heap_header, int heap_size, free_space_pointer* sfp){

    //释放空闲链表
    while (sfp != NULL){
        free_space_pointer* temp = sfp;
        sfp = sfp->next;
        free(temp);
    }
    
    free_space_pointer free_header;
    free_header.next = NULL;

    int i = 0;
    void* pre = heap_header;
    int gc_size = 0;
    int gc_all_size = 0;
    while (i < heap_size){
        if ((i + 8) < heap_size){
            int flag = *(int*)pre;
            int size = *(int*)(pre + 4);
            int object_all_size = sizeof(object_header) + size;
            if (flag == 0){
                gc_size = gc_size + object_all_size;
                pre = pre + object_all_size;
                
            }else{
                //添加空闲内存到空闲指针链表
                free_space_pointer* pointer = (free_space_pointer*) malloc(sizeof(free_space_pointer));
                pointer->size = gc_size;
                pointer->free_address = pre - gc_size;
                pointer->next = free_header.next;
                free_header.next = pointer;
                
                gc_all_size = gc_all_size + gc_size;
                gc_size = 0;

                pre = pre + object_all_size;
            }
            i = i + object_all_size;
        }else{
            free_space_pointer* pointer = (free_space_pointer*) malloc(sizeof(free_space_pointer));
            pointer->size = heap_size - i;
            pointer->free_address = pre;
            pointer->next = free_header.next;
            free_header.next = pointer;
            break;
        }
        
    }
    printf("总共释放了%dB堆空间\n", gc_all_size);
    return free_header.next;
}



void mark_object(void* object){
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

