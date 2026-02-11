#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum {
        AST_STATEMENTBLOCK, //array/heap-allocated array of tokens for statements
        AST_FUNCTIONDEFINITION,
        AST_FUNCTIONCALL,
        AST_IFBLOCK,
        AST_SWITCHBLOCK,
        AST_WHILE,
        AST_FOR,
        AST_DOWHILE,
} ASTIdentifier;

typedef struct ListNode {
        struct ListNode *next;
        struct ListNode *children;
        ASTIdentifier type;
        Token *tokens;
        int num_tokens;
        int num_children;
} ListNode;

#endif