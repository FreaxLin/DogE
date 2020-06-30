#ifndef _HEAP_H
#define _HEAP_H
#endif

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

void* find_free_space(free_space_pointer* header, int size);
void add_free_space(free_space_pointer* header, void* space_address, int size);
free_space_pointer* allocate_heap_space(int size);
void mark_object(void* object);
free_space_pointer* sweep(void* heap_header, int heap_size, free_space_pointer* sfp);
free_space_pointer* gc(void* heap_header, void** objects, int count, int heap_size, free_space_pointer* sfp);