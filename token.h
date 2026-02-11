#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
        TOKEN_IDENTIFIER,
        TOKEN_NUMBERLITERAL,
        TOKEN_EQUAL,
        TOKEN_ADD,
        TOKEN_SUB,
        TOKEN_MULT,
        TOKEN_DIV,
        TOKEN_POW,
        TOKEN_LEFTPAREN,
        TOKEN_RIGHTPAREN,
        TOKEN_LEFTBRACE,
        TOKEN_RIGHTBRACE,
        TOKEN_LEFTBRACKET,
        TOKEN_RIGHTBRACKET,
        TOKEN_TYPESPECIFIER,
        TOKEN_SEMICOLON,
        TOKEN_KEYWORD,
        TOKEN_COMMA,
        TOKEN_EQUALTO,
        TOKEN_NOTEQUAL,
        TOKEN_GREATERTHAN,
        TOKEN_GTE,
        TOKEN_LESSTHAN,
        TOKEN_LTE,
        TOKEN_POSTINCREMENT,
        TOKEN_PREINCREMENT,
        TOKEN_POSTDECREMENT,
        TOKEN_PREDECREMENT,
        TOKEN_DEREFERENCE,
        TOKEN_ADDRESSOF,
        TOKEN_OR,
        TOKEN_AND,
        TOKEN_XOR,
        TOKEN_BAND,
        TOKEN_BOR,
        TOKEN_BXOR,
        TOKEN_LEFTSHIFT,
        TOKEN_RIGHTSHIFT,
        TOKEN_EOF,
        TOKEN_BNOT,
        TOKEN_NOT,
} TokenEnum;

typedef struct Token {
        struct Token *next;
        struct Token *prev;
        const char *lexeme;
        TokenEnum token;
        int line;
} Token;

Token *get_tokens(FILE* file);

#endif