#include "tokens.h"
#include <map>

std::string typeToString(TokenType type) {
    switch (type) {
        case TokenType::SEMICOLON:     return "SEMICOLON";
        case TokenType::IF:            return "IF";
        case TokenType::THEN:          return "THEN";
        case TokenType::END:           return "END";
        case TokenType::REPEAT:        return "REPEAT";
        case TokenType::UNTIL:         return "UNTIL";
        case TokenType::IDENTIFIER:    return "IDENTIFIER";
        case TokenType::ASSIGN:        return "ASSIGN";
        case TokenType::READ:          return "READ";
        case TokenType::WRITE:         return "WRITE";
        case TokenType::LESSTHAN:      return "LESSTHAN";
        case TokenType::EQUAL:         return "EQUAL";
        case TokenType::PLUS:          return "PLUS";
        case TokenType::MINUS:         return "MINUS";
        case TokenType::MULT:          return "MULT";
        case TokenType::DIV:           return "DIV";
        case TokenType::OPENBRACKET:   return "OPENBRACKET";
        case TokenType::CLOSEDBRACKET: return "CLOSEDBRACKET";
        case TokenType::NUMBER:        return "NUMBER";
        case TokenType::END_OF_FILE:   return "END_OF_FILE";
        case TokenType::ERROR_TOKEN:   return "ERROR_TOKEN";
    }
    return "UNKNOWN";
}

TokenType lookupKeyword(const std::string& word) {
    static const std::map<std::string, TokenType> keywords = {
        {"if",     TokenType::IF},
        {"then",   TokenType::THEN},
        {"end",    TokenType::END},
        {"repeat", TokenType::REPEAT},
        {"until",  TokenType::UNTIL},
        {"read",   TokenType::READ},
        {"write",  TokenType::WRITE}
    };

    auto it = keywords.find(word);
    if (it != keywords.end()) {
        return it->second;
    }
    return TokenType::IDENTIFIER;
}