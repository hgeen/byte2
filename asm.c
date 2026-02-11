#include <stdlib.h>
#include "asm.h"
#include "ast.h"
#include "types.h"

// TODO: (maybe)
// add a way to do inline 'asm' (not actually asm, just manual bytecodes really)

// TODO: 
// byte2 libraries are added at the top of the assembly (or just somewhere else as a pointer)
// library function names are mangled in this file

typedef struct {
        int size;
        int capacity;
} WordSize;

ObjectContainer *assemble_instructions(ListNode *nodes, const char *name) {
        WORD *instructions = malloc(16 * sizeof(WORD));
        WordSize wsz = {.size = 16, .capacity = 0};

        ObjectContainer *container = malloc(sizeof(ObjectContainer));
        container->name = name;
        container->instructions = instructions;

        return container;
}