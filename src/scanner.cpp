#include "scanner.h"
#include <cctype>

Scanner::Scanner(const std::string& source)
    : source_(source), pos_(0), line_(1) {}

int Scanner::line() const { return line_; }

bool Scanner::atEnd() const { return pos_ >= source_.size(); }

char Scanner::peek() const { return atEnd() ? '\0' : source_[pos_]; }

char Scanner::advance() {
    if (atEnd()) return '\0';
    char c = source_[pos_++];
    if (c == '\n') line_++;
    return c;
}

void Scanner::skipWhitespaceAndComments() {
    while (!atEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            advance();
        } else if (c == '{') {
            size_t startPos = pos_;
            int startLine = line_;
            advance();                                  // the '{'
            while (!atEnd() && peek() != '}') advance();
            if (atEnd()) {                              // never closed:
                pos_ = startPos;                        // step back and leave
                line_ = startLine;                      // the '{' for getToken
                return;
            }
            advance();                                  // the '}'
        } else {
            return;
        }
    }
}

Token Scanner::scanWord() {
    std::string word;
    while (std::isalpha(static_cast<unsigned char>(peek()))) word += advance();
    return {word, lookupKeyword(word)};
}

Token Scanner::scanNumber() {
    std::string num;
    while (std::isdigit(static_cast<unsigned char>(peek()))) num += advance();
    return {num, TokenType::NUMBER};
}

Token Scanner::getToken() {
    skipWhitespaceAndComments();
    if (atEnd()) return {"", TokenType::END_OF_FILE};

    unsigned char c = static_cast<unsigned char>(peek());
    if (std::isalpha(c)) return scanWord();
    if (std::isdigit(c)) return scanNumber();

    advance();   // every remaining case consumes at least this one character
    switch (c) {
        case ';': return {";", TokenType::SEMICOLON};
        case '<': return {"<", TokenType::LESSTHAN};
        case '=': return {"=", TokenType::EQUAL};
        case '+': return {"+", TokenType::PLUS};
        case '-': return {"-", TokenType::MINUS};
        case '*': return {"*", TokenType::MULT};
        case '/': return {"/", TokenType::DIV};
        case '(': return {"(", TokenType::OPENBRACKET};
        case ')': return {")", TokenType::CLOSEDBRACKET};

        case ':':
            if (peek() == '=') {
                advance();
                return {":=", TokenType::ASSIGN};
            }
            return {":", TokenType::ERROR_TOKEN};      // lone colon

        case '{':                                       // only reachable if never closed
            while (!atEnd()) advance();
            return {"{", TokenType::ERROR_TOKEN};

        default: {
            std::string bad(1, static_cast<char>(c));
            if (c >= 0x80) {                            // keep a pasted symbol like '–' as one error
                while ((static_cast<unsigned char>(peek()) & 0xC0) == 0x80)
                    bad += advance();
            }
            return {bad, TokenType::ERROR_TOKEN};
        }
    }
}



