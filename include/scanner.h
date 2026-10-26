#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "tokens.h"

class Scanner {
public:
    explicit Scanner(const std::string& source);

    // Returns the next token each call; END_OF_FILE forever once input ends.
    Token getToken();

    // Line of the scanner's current position (for screen warnings).
    int line() const;

private:
    std::string source_;   // the whole input text
    size_t pos_;           // index of the next unread character
    int line_;             // current line number, starts at 1 

    char peek() const;     // look at the next char without consuming it ('\0' at end)
    char advance();        // consume the next char and return it
    bool atEnd() const;    // true when pos_ has reached the end

    void skipWhitespaceAndComments();   // spaces, newlines, and { ... }
    Token scanWord();      // letters -> keyword or IDENTIFIER
    Token scanNumber();    // digits -> NUMBER
};

#endif