#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

#define END_OF_BUFFER = (char*) -1

typedef enum {
    FAIL = 0,
    SUCCESS = 1,
} EXIT_CODE;

typedef struct {
    const char *lexeme;
    TokenEnum type;
} Keyword;

typedef struct {
        const char *src;
        size_t pos;
        unsigned int line;
} Lexer;

static const Keyword keywords[] = {
    {"if", TOKEN_KEYWORD},
    {"else", TOKEN_KEYWORD},
    {"for", TOKEN_KEYWORD},
    {"while", TOKEN_KEYWORD},
    {"do", TOKEN_KEYWORD},
    {"switch", TOKEN_KEYWORD},
    {"case", TOKEN_KEYWORD},
    {"return", TOKEN_KEYWORD},
    {"typedef", TOKEN_KEYWORD},
    {"class", TOKEN_KEYWORD},
    {"struct", TOKEN_KEYWORD},
    {"int", TOKEN_TYPESPECIFIER},
    {"float", TOKEN_TYPESPECIFIER},
    {"double", TOKEN_TYPESPECIFIER},
    {"char", TOKEN_TYPESPECIFIER},
    {"void", TOKEN_TYPESPECIFIER},
    {"unsigned", TOKEN_TYPESPECIFIER},
};

static TokenEnum punct_table[128] = {
    ['('] = TOKEN_LEFTPAREN,
    [')'] = TOKEN_RIGHTPAREN,
    ['{'] = TOKEN_LEFTBRACE,
    ['}'] = TOKEN_RIGHTBRACE,
    ['['] = TOKEN_LEFTBRACKET,
    [']'] = TOKEN_RIGHTBRACKET,
    [';'] = TOKEN_SEMICOLON,
    [','] = TOKEN_COMMA,
    ['+'] = TOKEN_ADD,
    ['-'] = TOKEN_SUB,
    ['*'] = TOKEN_MULT,
    ['/'] = TOKEN_DIV,
    ['='] = TOKEN_EQUAL,
    ['<'] = TOKEN_LESSTHAN,
    ['>'] = TOKEN_GREATERTHAN,
};

static char current = 0;

static EXIT_CODE text_to_int(const char *text, int *buffer) {
        int i = 0;
        char c = 0;
        char sign = 1;
        int result = 0;

        if (text[0] == '-') {
                sign = -1;
                i = 1;
        }

        while ((c = text[i++]) != '\n') {
                if (c - '0' > 9 || c - '0' < 0) {
                        return FAIL;
                }

                result = result * 10 + (c - '0');
        }

        // i assume this sets the value of buffer to result
        *buffer = result;

        return SUCCESS;
}

static EXIT_CODE text_to_float(const char *text, float *buffer) {
        int i = 0;
        char c = 0;
        char sign = 1;
        int result = 0;
        int fraction = 0;
        int factor = 1;
        char decimal = 0;

        if (text[0] == '-') {
                sign = -1;
                i = 1;
        }

        while ((c = text[i++])) {
                if (c == '.') {
                        decimal = 1;
                        break;
                } else if (c - '0' > 9 || c - '0' < 0) {
                return FAIL;
                }

                result = result * 10 + (c - '0');
        }

        if (decimal) {
                while ((c = text[i++]) != '\0') {
                if (c - '0' > 9 || c - '0' < 0) {
                        return FAIL;
                }

                fraction = fraction * 10 + (c - '0');
                factor *= 10;
                }
                
                *buffer = sign * ((float) result + ((float) fraction / factor));

                return SUCCESS;
        }

        *buffer = (float) result * sign;

        return SUCCESS;
}

char is_punct(char character) {
        switch (character) {
                case '(':
                case ')':
                case '{':
                case '}':
                case ';':
                case '*':
                case '+':
                case '-':
                case '/':
                case ',':
                case '=':
                case '<':
                case '>':
                case '[':
                case ']':
                return 1;
        }

        return 0;
}

char is_reserved(const char *lexeme) {
        for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
                if (strcmp(lexeme, keywords[i].lexeme) == 0) return 1;
        }

        return 0;
}

static inline char is_letter(char character) {
        //if (character < 'A' || character > 'Z' || character < 'a' || character > 'z' || character < '0' || character > '9') return 0;
        return ((character < 'A' || character > 'Z') || 
                (character < 'a' || character > 'z') ||
                (character == '_')) ? 1 : 0;
}

static inline char is_alpha(char character) {
        return (character == ' ' || character == '\t' || character == '\n') ? 1 : 0;
}

static inline char is_alnum(char character) {
        return ((character >= '0' && character <= '9') ||
                (character >= 'Z' && character <= 'Z') ||
                (character >= 'a' && character <= 'z')) ? 1 : 0;
}

static inline char advance(Lexer *l) {
        return l->src[l->pos++];
}

static inline char peek(Lexer *l) {
        return l->src[l->pos];
}

static inline char peek_behind(Lexer *l) {
        return l->src[l->pos - 1];
}

static inline char peek_next(Lexer *l) {
        return l->src[l->pos + 1];
}

static inline char match(Lexer *l, char expected) {
        if (expected != peek(l)) return 0;
        l->pos++;
        return 1;
}

Token *new_token(TokenEnum type, char *lexeme) {
        Token *t = malloc(sizeof(Token));
        t->token = type;
        t->lexeme = lexeme;
        t->next = NULL;
        t->prev = NULL;
        return t;
}

TokenEnum get_token_type(const char *lexeme) {
        for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
                if (strcmp(lexeme, keywords[i].lexeme) == 0) return keywords[i].type;
        }

        if (is_punct(*lexeme)) return punct_table[(unsigned char) *lexeme];

        void *data = malloc(sizeof(int));

        if (text_to_int(lexeme, data) || text_to_float(lexeme, data)) {
                return TOKEN_NUMBERLITERAL;
        }

        free(data);

        return TOKEN_IDENTIFIER;
}

// TODO:
// complete rewrite of this, this is terrible
Token *next_token(Lexer *l) {
        while (is_alpha(peek(l))) {
                if (peek(l) == '\n') l->line++;

                advance(l);
        }

        if (peek(l) == EOF) return new_token(TOKEN_EOF, NULL);
        
        // identifiers & keywords
        else if (is_letter(peek(l)) || peek(l) == '_') {
                Token *t = malloc(sizeof(Token));
                char c = 0;
                int i = 0;
                int size = 64;
                char *lexeme = malloc(sizeof(char));

                while ((!is_alpha(c = advance(l)) || c != EOF) && (is_alnum(c) || c == '_')) {
                        if (c == '\n') {
                                l->line++;
                                continue;
                        }

                }
        }

        char c = advance(l);

        switch (c) {
                case '!':
                        if (match(l, '=')) {
                                return new_token(TOKEN_NOTEQUAL, NULL);
                        }
                        else {
                                return new_token(TOKEN_NOT, NULL);
                        }
                case '=':
                        if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_EQUALTO, NULL);
                        } else {
                                return new_token(TOKEN_EQUAL, NULL);
                        }
                case '>':
                        if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_GTE, NULL);
                        } else if (match(l, '>')) {
                                advance(l);
                                return new_token(TOKEN_RIGHTSHIFT, NULL);
                        } else {
                                return new_token(TOKEN_GREATERTHAN, NULL);
                        }
                case '<':
                        if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_LTE, NULL);
                        } else if (match(l, '>')) {
                                advance(l);
                                return new_token(TOKEN_LEFTSHIFT, NULL);
                        } else {
                                return new_token(TOKEN_LESSTHAN, NULL);
                        }
                case '*':
                        if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_MULTEQUAL, NULL);
                        } else {
                                return new_token(TOKEN_DEREFORMULT, NULL);
                        }
                case '&':
                        if (match(l, '&')) {
                                advance(l);
                                return new_token(TOKEN_AND, NULL);
                        } else if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_BANDEQ, NULL);
                        } else {
                                return new_token(TOKEN_BAND, NULL);
                        }
                case '^':
                        if (match(l, '^')) {
                                advance(l);
                                return new_token(TOKEN_XOR, NULL);
                        } else if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_XOREQ, NULL);
                        } else {
                                return new_token(TOKEN_BXOR, NULL);
                        }
                case '|':
                        if (match(l, '|')) {
                                advance(l);
                                return new_token(TOKEN_OR, NULL);
                        } else if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_BOREQ, NULL);
                        } else {
                                return new_token(TOKEN_BOR, NULL);
                        }
                case '+':
                        if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_ADDEQUAL, NULL);
                        } else if (match(l, '+') && is_letter(peek_next(l))) {
                                advance(l);
                                return new_token(TOKEN_PREINCREMENT, NULL);
                        } else if (match(l, '+') && !is_alpha(peek_behind(l))) {
                                return new_token(TOKEN_POSTINCREMENT, NULL);
                        } else {
                                return new_token(TOKEN_ADD, NULL);
                        }
                case '-':
                        if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_SUBEQUAL, NULL);
                        } else if (match(l, '-') && is_letter(peek_next(l))) {
                                advance(l);
                                return new_token(TOKEN_PREDECREMENT, NULL);
                        } else if (match(l, '-') && !is_alpha(peek_behind(l))) {
                                advance(l);
                                return new_token(TOKEN_POSTDECREMENT, NULL);
                        } else if (match(l, '>')) {
                                advance(l);
                                return new_token(TOKEN_ARROWDEREF, NULL);
                        } else {
                                return new_token(TOKEN_SUB, NULL);
                        }
                case '/':
                        if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_DIVEQUAL, NULL);
                        } else {
                                return new_token(TOKEN_DIV, NULL);
                        }
                case '~':
                        if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_BNOTEQ, NULL);
                        } else {
                                return new_token(TOKEN_BNOT, NULL);
                        }
                case '%':
                        if (match(l, '=')) {
                                advance(l);
                                return new_token(TOKEN_MODEQ, NULL);
                        } else {
                                return new_token(TOKEN_MOD, NULL);
                        }
                case '(':
                        return new_token(TOKEN_LEFTPAREN, NULL);
                case ')':
                        return new_token(TOKEN_RIGHTPAREN, NULL);
                case '{':
                        return new_token(TOKEN_LEFTBRACE, NULL);
                case '}':
                        return new_token(TOKEN_RIGHTBRACE, NULL);
                case '[':
                        return new_token(TOKEN_LEFTBRACKET, NULL);
                case ']':
                        return new_token(TOKEN_RIGHTBRACKET, NULL);
                case ';':
                        return new_token(TOKEN_SEMICOLON, NULL);
                case ',':
                        return new_token(TOKEN_COMMA, NULL);
        }
}

Token *get_tokens(FILE *file) {
        Lexer l = {.src = NULL, .pos = 0};
        Token *head = malloc(sizeof(Token));
        Token *current = head;
        Token *prev = NULL;

        if (peek(&l) != EOF) {
                head = next_token(&l);
        }

        while (1) {
                Token *t = next_token(&l);
                current->next = t;

                if (current == head) {
                        current->prev = NULL;
                } else {
                        current->prev = prev;
                }

                if (t->token == TOKEN_EOF) current->next = NULL;

                prev = current;
                current = t;
        }

        return head;
}