// main.cpp - entry point and output formatting for the TINY scanner project.
//
// Usage:
//   scanner                      (asks for the input/output file names)
//   scanner input.txt            (writes tokens to output.txt)
//   scanner input.txt out.txt
//
// Output file : one line per valid token, in the form  "value , TYPE"
//               e.g.   x , IDENTIFIER
//                      := , ASSIGN
//                      4 , NUMBER
// Screen      : warnings for ERROR_TOKENs (bad characters / malformed words);
//               these are NOT written to the output file.

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "scanner.h"
#include "tokens.h"

// Reads the whole file into a string. Returns false if it cannot be opened.
static bool readFile(const std::string& path, std::string& out) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    out.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    return true;
}

// One line of the output file: "value , TYPE"
static void writeToken(std::ostream& os, const Token& tok) {
    os << tok.value << " , " << typeToString(tok.type) << '\n';
}

// Warning printed on screen for an invalid token.
static void warnError(const Token& tok, int line) {
    std::cout << "[Warning] Line " << line
              << ": invalid token '" << tok.value << "' (skipped)\n";
}

int main(int argc, char* argv[]) {
    std::string inputPath;
    std::string outputPath = "output.txt";

    if (argc >= 2) {
        inputPath = argv[1];
        if (argc >= 3) outputPath = argv[2];
    } else {
        std::cout << "Enter input file name: ";
        std::getline(std::cin, inputPath);
        std::cout << "Enter output file name (leave empty for output.txt): ";
        std::string tmp;
        std::getline(std::cin, tmp);
        if (!tmp.empty()) outputPath = tmp;
    }

    std::string source;
    if (!readFile(inputPath, source)) {
        std::cerr << "Error: cannot open input file '" << inputPath << "'\n";
        return 1;
    }

    std::ofstream out(outputPath);
    if (!out) {
        std::cerr << "Error: cannot create output file '" << outputPath << "'\n";
        return 1;
    }

    Scanner scanner(source);
    int tokenCount = 0;
    int errorCount = 0;

    while (true) {
        Token tok = scanner.getToken();

        if (tok.type == TokenType::END_OF_FILE) break;

        if (tok.type == TokenType::ERROR_TOKEN) {
            warnError(tok, scanner.line());
            ++errorCount;
            continue;
        }

        writeToken(out, tok);
        ++tokenCount;
    }

    out.close();

    std::cout << "\nScanning finished: " << tokenCount << " token(s) written to '"
              << outputPath << "', " << errorCount << " warning(s).\n";

    // Keep the console open when the exe is double-clicked on Windows.
    if (argc < 2) {
        std::cout << "Press Enter to exit...";
        std::cin.get();
    }
    return 0;
}
