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

static TokenEnum punct_table[256] = {
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

char is_punct(const char character) {
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

char is_letter(const char character) {
        if (character < 'A' || character > 'Z' || character < 'a' || character > 'z' || character < '0' || character > '9') return 0;
        return 1;
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

Token *new_token(TokenEnum type, char *lexeme) {
        Token *t = malloc(sizeof(Token));
        t->token = type;
        t->lexeme = lexeme;
        return t;
}

// converts == to ==, creates >= and <= operators
void adjust_tokens(Token *tokens) {

}

Token *get_tokens(FILE *file) {
        Token *tokens = NULL;
        Token *tail = NULL;
        char *lexeme = malloc(sizeof(char) * 64);
        int i = 0;
        int line = 1;
        char eof = 0;

        while((current = fgetc(file))) {
                if (current == '\n') line++;

                if (current == ' ' || !is_letter(current) || current == EOF) {
                        if (i == 0) continue;

                        lexeme[i] = '\0';
                        i = 0;

                        TokenEnum type = get_token_type(lexeme);
                        Token *new_tok = NULL;

                        new_tok = new_token(type, lexeme);

                        if (tokens == NULL) {
                                tokens = new_tok;
                                tail = new_tok;
                        } else {
                                tail->next = new_tok;
                                tail = new_tok;
                        }

                        if (is_punct(current)) {
                                TokenEnum t2 = get_token_type(&current);
                                Token *punct_token = new_token(t2, &current);
                                tail->next = punct_token;
                                tail = punct_token;
                        }

                        if (current == EOF) break;
                } else {
                        lexeme[i++] = current;
                }
        }

        adjust_tokens(tokens);

        return tokens;
}