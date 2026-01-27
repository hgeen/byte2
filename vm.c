#include <stdio.h>
#include <stdlib.h>
#include "asm.h"

// EVERYTHING IN THE VM IS SUBJECT TO CHANGE

typedef enum {
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_CHAR,
        TYPE_CLASS,
        TYPE_STRUCT,
        TYPE_FUNCTION,
} ValueType;

typedef union {
        int i;
        int *i_arr;
        float f;
        float *f_arr;
        char c;
        char *c_arr;
        void* dataPtr;
        int *funcPtr;
} Value;

typedef struct {
        Value value;
        ValueType type;
} ObjectValue;

typedef struct Scope {
        struct Scope *down;
        struct Scope *up;
        ObjectValue stackObjects[256];
        ObjectValue *heapObjects;
        int stack_i;
        int heap_i;
} Scope;

typedef struct ReturnPointer {
        struct ReturnPointer *next;
        struct ReturnPointer *prev;
} ReturnPointer;

Scope global;
int *returnPtrs = NULL;

void add_var(Scope *scope, ValueType type, Value value) {

}

void add_heap_var(Scope *scope, ValueType type, Value value) {

}

void init_vm() {
        global.down = NULL;
        global.up = NULL;
        global.stack_i = 0;
        global.heap_i = 0;
        global.heapObjects = malloc(256 * sizeof(ObjectValue));
}

void run_vm(int* bytes) {

}