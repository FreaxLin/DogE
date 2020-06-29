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