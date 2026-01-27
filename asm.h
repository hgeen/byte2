#ifndef ASM_H
#define ASM_H

typedef enum {
        OP_PUSH,
        OP_POP,
        OP_STORE,
        OP_DEALLOC,
        OP_ADD,
        OP_SUB,
        OP_MULT,
        OP_DIV,
        OP_POW,
        OP_FUNCDEF,
        OP_CALL,
        OP_JZE,
        OP_JNZE,
        OP_JL,
        OP_JG,
        OP_JLE,
        OP_JGE,
        OP_IMPORT,
        OP_DATACREATE,
        OP_NEWUNIT,
        OP_HEAPDATA, // for arrays and data created on the heap
        OP_STACKARRAY,
        OP_RETURN,
} OPCode;

#endif