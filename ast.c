#include <stdlib.h>
#include "token.h"
#include "ast.h"

// done iteratively to avoid blowing up the stack (assuming a huge file is passed creating a lot of tokens)
void free_tokens(Token *t) {
        Token *cursor = t;
        Token *temp = NULL;

        while (cursor->next != NULL) {
                cursor = cursor->next;
        }

        while (cursor != NULL) {
                temp = cursor->prev;
                        free((void *) cursor->lexeme);
                free(cursor);
                cursor = temp;
        }
}

ListNode *get_nodes(Token* t) {
        ListNode *nodes;

        free_tokens(t);

        return nodes;
}