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
        TOKEN_GREATERTHAN,
        TOKEN_GTE,
        TOKEN_LESSTHAN,
        TOKEN_LTE,
        TOKEN_POSTINCREMENT,
        TOKEN_PREINCREMENT,
        TOKEN_POSTDECREMENT,
        TOKEN_PREDECREMENT
} TokenEnum;

typedef struct Token {
        struct Token *next;
        TokenEnum token;
        const char *lexeme;
        int line;
} Token;

typedef struct {
        const char *src;
        char current;
        char *lexeme;
} Lexer;

Token *get_tokens(FILE* file);

#endif