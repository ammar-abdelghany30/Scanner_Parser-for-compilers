# TINY Language Scanner

A lexical analyzer (scanner) for the **TINY** language, written in C++ as part of the compiler course project. It reads TINY source code, splits it into tokens, and writes the list of `(token value, token type)` pairs to a file. Invalid tokens are reported as warnings on the screen.

## Features

- `getToken()` returns the next token from the input, together with its string value and type.
- Writes every valid token to an output file, one per line, in the form `value , TYPE`.
- Skips whitespace and `{ ... }` comments.
- Reports invalid input (bad characters, malformed words/numbers, unclosed comments) as screen warnings with the line number, without stopping the scan.
- Runs as a standalone Windows `.exe` with no third-party tools.

## Token Types

| Token type      | Example / Value |
|-----------------|-----------------|
| `SEMICOLON`     | `;`             |
| `IF`            | `if`            |
| `THEN`          | `then`          |
| `ELSE`          | `else`          |
| `END`           | `end`           |
| `REPEAT`        | `repeat`        |
| `UNTIL`         | `until`         |
| `IDENTIFIER`    | `x`, `abc`, `xyz` |
| `ASSIGN`        | `:=`            |
| `READ`          | `read`          |
| `WRITE`         | `write`         |
| `LESSTHAN`      | `<`             |
| `EQUAL`         | `=`             |
| `PLUS`          | `+`             |
| `MINUS`         | `-`             |
| `MULT`          | `*`             |
| `DIV`           | `/`             |
| `OPENBRACKET`   | `(`             |
| `CLOSEDBRACKET` | `)`             |
| `NUMBER`        | `12`, `289`     |

Two internal types are not written to the output file:

- `END_OF_FILE`: tells `main.cpp` the input is finished.
- `ERROR_TOKEN`: an invalid token, reported on screen only.

## Lexical Rules

- **Keywords** are lowercase: `if`, `then`, `else`, `end`, `repeat`, `until`, `read`, `write`.
- **Identifiers** consist of letters only. A word that contains a digit (for example `abc1`) is an error.
- **Numbers** consist of digits only. A number followed by letters (for example `12x`) is an error.
- **Comments** are written between `{` and `}`. A comment that is never closed is reported as an error.
- A lone `:` (not followed by `=`) is an error.
- Any other unrecognized character (for example `@`, `#`) is an error.

## Project Structure

```
compiler_project/
├── include/
│   ├── scanner.h     Scanner class declaration
│   └── tokens.h      TokenType enum, Token struct, helper declarations
├── src/
│   ├── main.cpp      Entry point, file handling, output formatting
│   ├── scanner.cpp   Scanner implementation (getToken, scanWord, scanNumber, ...)
│   └── tokens.cpp    typeToString and keyword lookup
├── test/             Sample TINY input files
├── docs/             Project description, report, screenshots
├── README.md
└── LICENSE
```

## Requirements

- A C++11 compiler. This project was built with **g++ (MSYS2 / MinGW-w64)** on Windows.

## Build

Run from the project root folder.

```
g++ -std=c++11 -Iinclude src/main.cpp src/scanner.cpp src/tokens.cpp -o scanner.exe
```

To build an executable that runs on any Windows PC without extra DLLs, add `-static`:

```
g++ -std=c++11 -static -Iinclude src/main.cpp src/scanner.cpp src/tokens.cpp -o scanner.exe
```

> Tip: if the compiler prints nothing and `scanner.exe` is not created when using PowerShell with Anaconda active, run `conda deactivate` first, or build from the **MSYS2 UCRT64** terminal.

## Usage

```
scanner.exe <input file> [output file]
```

| Arguments given | Behavior |
|-----------------|----------|
| none            | Asks for the input and output file names. Press Enter at the output prompt to use `output.txt`. |
| input only      | Writes tokens to `output.txt`. |
| input and output | Writes tokens to the given output file. |

Examples:

```
.\scanner.exe test\input.txt
.\scanner.exe test\input.txt tokens.txt
```

(In MSYS2 or Linux shells use `./scanner.exe test/input.txt`.)

## Example

**Input** (`test/input.txt`):

```
{ sample program }
read x;
if x < 10 then
  write x + @
end
```

**Screen output:**

```
[Warning] Line 4: invalid token '@' (skipped)

Scanning finished: 11 token(s) written to 'output.txt', 1 warning(s).
```

**Output file** (`output.txt`):

```
read , READ
x , IDENTIFIER
; , SEMICOLON
if , IF
x , IDENTIFIER
< , LESSTHAN
10 , NUMBER
then , THEN
write , WRITE
x , IDENTIFIER
+ , PLUS
end , END
```

## How It Works

1. `main.cpp` reads the whole input file into a string and creates a `Scanner` from it.
2. It calls `getToken()` repeatedly until `END_OF_FILE` is returned.
3. For each token:
   - valid tokens are written to the output file with `typeToString`;
   - `ERROR_TOKEN`s print a warning (with `scanner.line()`) and are skipped.
4. A summary with the token and warning counts is printed at the end.

Inside the scanner, `getToken()` skips whitespace and comments, then decides by the first character:

- a letter starts `scanWord()`, which returns a keyword or an identifier;
- a digit starts `scanNumber()`;
- anything else is matched as a symbol (`;`, `<`, `=`, `+`, `-`, `*`, `/`, `(`, `)`, `:=`) or reported as an error.

## Team
All members are students at the Faculty of Engineering, Ain Shams University.

- Ammar Mohamed
- Amr Wahidi
- Ali Abdelmaboud
- Belal Ahmed
- Omar Mohamed
- Hamza Basem
