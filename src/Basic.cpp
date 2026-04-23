#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      // Tokenize the input line
      TokenStream tokens = lexer.tokenize(line);

      // Check for interpreter commands
      const Token* firstToken = tokens.peek();
      if (firstToken) {
        if (firstToken->type == TokenType::RUN) {
          program.run();
          continue;
        } else if (firstToken->type == TokenType::LIST) {
          program.list();
          continue;
        } else if (firstToken->type == TokenType::CLEAR) {
          program.clear();
          continue;
        } else if (firstToken->type == TokenType::QUIT) {
          break;
        } else if (firstToken->type == TokenType::HELP) {
          // Print help message (optional)
          continue;
        }
      }

      // Parse the line
      ParsedLine parsed = parser.parseLine(tokens, line);

      // Check if it's a line with number
      std::optional<int> lineNum = parsed.getLine();
      Statement* stmt = parsed.fetchStatement();

      if (lineNum.has_value()) {
        // It's a numbered line
        if (stmt == nullptr) {
          // Just a line number - delete that line
          program.removeStmt(lineNum.value());
        } else {
          // Add or replace the line
          program.addStmt(lineNum.value(), stmt);
        }
      } else {
        // It's an immediate command
        if (stmt != nullptr) {
          program.execute(stmt);
          delete stmt;
        }
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}