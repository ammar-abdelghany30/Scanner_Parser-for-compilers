# TINY Scanner

A lexical analyzer (scanner) for the **TINY** language, written in C++.
It reads a TINY code snippet and breaks it into tokens.

> Design of Compilers course project, Faculty of Engineering, Ain Shams University.

---

## Objective

Turn raw TINY source code into an ordered list of `(token value, token type)` pairs, so that a later stage (the parser) can work with tokens instead of raw characters.

The project delivers:

1. A **`getToken`** function that returns the next token from the input, together with its string value.
2. An **output file** listing every token as `value , TYPE`.
3. A Windows **executable**, snapshots of the program running, and the code files.

## Scope

**In scope**

- Recognizing the 20 token types required by the course document (table below).
- Skipping whitespace and `{ comments }`.
- Reporting invalid input on the screen without stopping the scan.

**Out of scope**

- Checking whether the program makes sense (for example `if if ;`). Every piece is a valid token, and checking their order belongs to the parser in the next project.

### Token types

| Token type | Example | Token type | Example |
|---|---|---|---|
| SEMICOLON | `;` | PLUS | `+` |
| IF | `if` | MINUS | `-` |
| THEN | `then` | MULT | `*` |
| END | `end` | DIV | `/` |
| REPEAT | `repeat` | OPENBRACKET | `(` |
| UNTIL | `until` | CLOSEDBRACKET | `)` |
| READ | `read` | LESSTHAN | `<` |
| WRITE | `write` | EQUAL | `=` |
| ASSIGN | `:=` | NUMBER | `12`, `289` |
| IDENTIFIER | `x`, `abc`, `xyz` | ELSE | `else`

## How it works

The scanner reads the text one character at a time, like a finger moving along a line. Each call to `getToken` skips whitespace and comments, looks at the next character, and cuts out one token.

| Next character | Result |
|---|---|
| A letter | Collect the word, then look it up: keyword (`if`, `then`, ...) or IDENTIFIER |
| A digit | Collect the digits into a NUMBER |
| `:` followed by `=` | ASSIGN |
| One of `; < = + - * / ( )` | The matching symbol token |
| `{` | Skip everything up to `}` (a comment) |
| Anything else | An error token, then scanning continues |

### Error handling

- Errors are **printed on the screen** and are **not written** to the output file, so `output.txt` contains only valid tokens.
- The scanner never crashes or hangs. It reports the problem and keeps going.
- Detected errors: an illegal character (`@`, `#`, `!`), a lone `:`, an unterminated comment, and a word that mixes letters and digits (`aar12you`, `x1`, `12abc`), which is reported as one invalid token.

## Architecture

```
input.txt --> main.cpp --> Scanner::getToken() --> Token --> OutputWriter --> output.txt
                                  |
                       uses tokens.h / tokens.cpp
                       (token types, keyword table)
```

| Component | Role |
|---|---|
| `tokens.h` / `tokens.cpp` | The vocabulary: the `TokenType` enum, the `Token` struct, the keyword table, and a function that turns a type into its name |
| `scanner.h` / `scanner.cpp` | The behavior: the `Scanner` class remembers its position in the text, and `getToken` finds the next token |
| `output_writer.h` / `.cpp` | Formats one token as a `value , TYPE` line and writes it to the file |
| `main.cpp` | The driver: reads the input file, loops over `getToken` until the end, sends valid tokens to the writer and errors to the screen |

Only `Token` objects pass between the components, so each part can be developed and tested separately.

## Repository structure

```
tiny-scanner/
├── README.md
├── Makefile
├── input.txt                 # sample input (factorial program)
├── include/
│   ├── tokens.h
│   ├── scanner.h
│   └── output_writer.h
├── src/
│   ├── tokens.cpp
│   ├── scanner.cpp
│   ├── output_writer.cpp
│   └── main.cpp
├── tests/                    # input files, expected outputs, scanner_test.cpp
├── docs/                     # interface contract, DFA sketch, report
├── snapshots/                # screenshots of the program running
└── dist/                     # scanner.exe
```

## Build and run

Requires a C++17 compiler (MinGW `g++`). Run the commands from the repository root.

```
mingw32-make
dist\scanner.exe input.txt output.txt
```

Without a Makefile:

```
g++ -std=c++17 -static -Iinclude src/*.cpp -o dist/scanner.exe
```

The `-static` flag bundles the runtime into the exe, so it runs on a clean Windows machine with nothing installed. Running the exe with no arguments reads `input.txt` and writes `output.txt`.

## Example

Input:

```
{ computes part of a factorial }
read x;
y := (x + 12) / 3;
```

Output (`output.txt`):

```
read , READ
x , IDENTIFIER
; , SEMICOLON
y , IDENTIFIER
:= , ASSIGN
( , OPENBRACKET
x , IDENTIFIER
+ , PLUS
12 , NUMBER
) , CLOSEDBRACKET
/ , DIV
3 , NUMBER
; , SEMICOLON
```

## Testing

`scanner_test.cpp` tests the `Scanner` class on its own, without `main.cpp`. It covers keywords, symbols, comments, error cases, line counting, and the instructor's factorial sample.

```
g++ -std=c++17 -Wall -Wextra -Iinclude src/tokens.cpp src/scanner.cpp tests/scanner_test.cpp -o tests/scanner_test.exe
tests\scanner_test.exe
```

## Team

All members are students at the Faculty of Engineering, Ain Shams University.

- Ammar Mohamed
- Amr Wahidi
- Ali Abdelmaboud
- Belal Ahmed
- Omar Mohamed
- Hamza Basem