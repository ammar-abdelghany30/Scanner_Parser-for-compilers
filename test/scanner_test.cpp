#include <iostream>
#include <string>
#include <algorithm>
#include "tokens.h"
#include "scanner.h"

static int failures = 0;

// Runs the scanner on src and returns "value,TYPE value,TYPE ..." (no EOF).
static std::string scanAll(const std::string& src) {
    Scanner s(src);
    std::string out;
    while (true) {
        Token t = s.getToken();
        if (t.type == TokenType::END_OF_FILE) break;
        if (!out.empty()) out += ' ';
        out += t.value + "," + typeToString(t.type);
    }
    return out;
}

static void report(const std::string& name, bool ok,
                   const std::string& expected, const std::string& got) {
    if (ok) { std::cout << "PASS  " << name << "\n"; return; }
    ++failures;
    std::cout << "FAIL  " << name << "\n  expected: " << expected
              << "\n  got:      " << got << "\n";
}

static void check(const std::string& name, const std::string& src,
                  const std::string& expected) {
    std::string got = scanAll(src);
    report(name, got == expected, expected, got);
}

int main() {
    // valid tokens
    check("assign with spaces", "x := 4;",
          "x,IDENTIFIER :=,ASSIGN 4,NUMBER ;,SEMICOLON");
    check("assign no spaces", "x:=4;",
          "x,IDENTIFIER :=,ASSIGN 4,NUMBER ;,SEMICOLON");
    check("all keywords", "read write else if then end repeat until",
          "read,READ write,WRITE else,ELSE if,IF then,THEN end,END repeat,REPEAT until,UNTIL");
    check("all symbols", "; < = + - * / ( )",
          ";,SEMICOLON <,LESSTHAN =,EQUAL +,PLUS -,MINUS *,MULT /,DIV (,OPENBRACKET ),CLOSEDBRACKET");
    check("expression", "a := (b + 12) / 289;",
          "a,IDENTIFIER :=,ASSIGN (,OPENBRACKET b,IDENTIFIER +,PLUS 12,NUMBER ),CLOSEDBRACKET /,DIV 289,NUMBER ;,SEMICOLON");
    check("keyword lookalikes", "If thenx",
          "If,IDENTIFIER thenx,IDENTIFIER");

    // nothing to output
    check("empty input", "", "");
    check("only whitespace", "  \n\t \r\n ", "");

    // comments
    check("comment only", "{x}", "");
    check("comment then word", "{x} x", "x,IDENTIFIER");
    check("comment between tokens", "a {skip <= me} b", "a,IDENTIFIER b,IDENTIFIER");
    check("unterminated comment", "x { oops", "x,IDENTIFIER {,ERROR_TOKEN");
    check("stray closing brace", "}", "},ERROR_TOKEN");

    // errors
    check("illegal char", "x := 4 @ 5;",
          "x,IDENTIFIER :=,ASSIGN 4,NUMBER @,ERROR_TOKEN 5,NUMBER ;,SEMICOLON");
    check("lone colon", "y : 3", "y,IDENTIFIER :,ERROR_TOKEN 3,NUMBER");
    check("letters+digits", "aar12you", "aar12you,ERROR_TOKEN");
    check("x1", "x1", "x1,ERROR_TOKEN");
    check("12abc", "12abc", "12abc,ERROR_TOKEN");
    check("en dash outside comment", "x \xE2\x80\x93 1",
          "x,IDENTIFIER \xE2\x80\x93,ERROR_TOKEN 1,NUMBER");

    // line counting
    {
        Scanner s("a\nb\n@");
        s.getToken(); s.getToken(); s.getToken();
        report("line number after error", s.line() == 3, "3", std::to_string(s.line()));
    }

    // the instructor's sample: 32 tokens, no errors
    {
        const std::string sample = R"TINY({ Sample program in TINY language – computes factorial}
read x; {input an integer }
if 0 < x then { don’t compute if x <= 0 }
  fact := 1;
  repeat
    fact := fact * x;
    x := x - 1
  until x = 0;
  write fact { output factorial of x }
end)TINY";
        std::string got = scanAll(sample);
        int n = got.empty() ? 0 : 1 + (int)std::count(got.begin(), got.end(), ' ');
        bool ok = (n == 32) && got.find("ERROR_TOKEN") == std::string::npos;
        report("instructor sample (32 tokens, 0 errors)", ok, "32 tokens",
               std::to_string(n) + " tokens");
    }

    std::cout << (failures == 0 ? "\nAll tests passed.\n"
                                : "\n" + std::to_string(failures) + " test(s) failed.\n");
    return failures == 0 ? 0 : 1;
}