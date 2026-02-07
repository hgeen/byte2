#ifndef ERROR_H
#define ERROR_H

typedef enum {
        SYNTAX,
        LINKER,
        ARITHMETIC
} ErrorCategory;

void create_error(ErrorCategory cat, int id);

#endif