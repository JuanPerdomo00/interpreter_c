#ifndef KIND_H
#define KIND_H

typedef enum TokenKind TokenKind;

enum TokenKind {
    // Básicos
    TOKEN_EOF,
    TOKEN_ERROR,

    // Identificadores y literales
    TOKEN_IDENTIFIER,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_STRING_LITERAL,

    // Keywords
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_FUNC,
    TOKEN_FOR,

    // Operadores
    TOKEN_PLUS,
    TOKEN_MINUS,

    TOKEN_STAR,
    TOKEN_STAR_STAR,

    TOKEN_SLASH,
    TOKEN_SLASH_SLASH,
    TOKEN_ASSIGN,

    TOKEN_EQUAL,

    TOKEN_PLUS_PLUS,
    TOKEN_MINUS_MINUS,

    // Delimitadores
    TOKEN_COMMA,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
};


char* kind_debug(TokenKind kind);

#endif
