#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "asm.h"
#include "types.h"

// EVERYTHING IN THE VM IS SUBJECT TO CHANGE

// HOW TO HANDLE RETURN VALUES:
// Once you return a value, push it onto the stack of the scope above the current scope and free it off the current one

typedef enum {
        VARIABLE,
        FUNCTION,
        DATA,

} IdentifierType;

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
        Value *value;
        ValueType type;
} ObjectValue;

// TODO:
// change variables so that they just point to an index on the stack
// structs/classes have a hash specifically for structs/classes to get correct offsets
// readtypes inside each variables

/*typedef struct Scope {
        struct Scope *down;
        struct Scope *up;
        ObjectValue stackObjects[256];
        ObjectValue *heapObjects;
        int stack_i;
        int heap_i;
} Scope;*/

// to replace direct index arrays so that variables can be indexed by their names
// this is just a hash table, and the collision detection is only to check if two
// variables have the same name
typedef struct {

} IdentifierTable;

// create an initializer list (this is a poor name because it is not what it actually is)
// what the initializer list does:
// keeps tracks of which variables are created and what their sizes are
// this could also just be a heap allocated array/the identifier table
// this is useful to index based directly on the stack pointer (amazing! now we can write assembly like code!)

typedef struct Scope {
        struct Scope *down;
        struct Scope *up;
} Scope;

typedef struct ReturnPointer {
        struct ReturnPointer *next;
        struct ReturnPointer *prev;
        BYTE current;
} ReturnPointer;

Scope global;
//int *return_ptrs = NULL;
ReturnPointer *ret_ptrs = NULL;

// TODO:
// create a function (possibly recursive) to fetch the offset from a struct/class to access a specific
// member value so that the languages bytecode can function similar to assembly

// TODO: (maybe)
// functions exist on the stack, just like everything else
// however functions are different because then that stack value points to a function definition in
// the instructions

// no need to check for negative numbers because since this is exclusively for bitshifts,
// analysis is performed in semantic.c to make sure you don't bitshift negatively
static inline char is_pow2(int x) {
        return (x & (x - 1)) == 0;
}

void add_var(Scope *scope, ValueType type, Value value) {

}

void add_heap_var(Scope *scope, ValueType type, Value value) {

}

void init_vm() {
        global.down = NULL;
        global.up = NULL;
        //global.stack_i = 0;
        //global.heap_i = 0;
        //global.heapObjects = malloc(256 * sizeof(ObjectValue));
}

// how to handle jump instructions:
// if the condition is met, the instruction index/pointer is set back by 4 and starts running there

// how to set up do-while loop:
// while loop body
// jump if condition to above loop

// how to set up while loop:
// if condition false jump ahead
// while loop body
// jump if condition to above loop

// how to set up while loop:
// if condition false jump ahead
// while loop body
// jump if condition to above loop

typedef struct {
        ObjectContainer *source;
        ObjectContainer *imports;
} Environment;

void run_vm(ObjectContainer *object_files) {

}