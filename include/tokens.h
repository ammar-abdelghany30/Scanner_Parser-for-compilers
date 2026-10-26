#ifndef TOKENS_H
#define TOKENS_H

#include <string>

enum class TokenType {
    SEMICOLON, IF, THEN, END, REPEAT, UNTIL,
    IDENTIFIER, ASSIGN, READ, WRITE,
    LESSTHAN, EQUAL, PLUS, MINUS, MULT, DIV,
    OPENBRACKET, CLOSEDBRACKET, NUMBER,
    END_OF_FILE,   // internal: tells main.cpp the input is finished
    ERROR_TOKEN    // internal: bad character, reported on screen only
};

struct Token {
    std::string value;   // exact text, e.g. ":=" or "abc"
    TokenType type;
};

// "IDENTIFIER" for TokenType::IDENTIFIER, etc.
std::string typeToString(TokenType type);

// Returns the keyword's type, or IDENTIFIER if the word is not a keyword.
TokenType lookupKeyword(const std::string& word);

#endif


