#include <stdlib.h>
#include "asm.h"
#include "ast.h"
#include "types.h"

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