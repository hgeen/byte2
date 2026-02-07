#ifndef ASM_H
#define ASM_H

#include "types.h"

typedef enum {
        OP_STOP,
        OP_RETURN,
        OP_PUSH,
        OP_POP,
        OP_STORE,
        OP_FREE, // like free()
        OP_ADD,
        OP_SUB,
        OP_MULT,
        OP_DIV,
        OP_POW,
        OP_FUNCDEF,
        OP_CALL,
        OP_JZE, //jump zero or equal
        OP_JNZE, //jump not zero or not equal
        OP_JL, //jump less than
        OP_JG, //jump greater than
        OP_JLE, //jump less than or equal to
        OP_JGE, //jump greater than or equal to
        OP_JT, //jump if true
        OP_JMP, //jump to label
        OP_IMPORT,
        //OP_DATACREATE, //create data type // not needed because i can just create it them at compile time and access them during
        // runtime to avoid extra unnecessary instructions
        OP_ALLOC, // for heap allocated stuff
        OP_STACKARRAY,
        OP_END,
        OP_LABEL,
} OPCode;

typedef struct {
        const char *name;
        WORD *instructions;
} ObjectContainer;

#endif