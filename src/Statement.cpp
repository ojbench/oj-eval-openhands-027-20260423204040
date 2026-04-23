#include "Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

// LetStatement
LetStatement::LetStatement(std::string source, std::string var,
                           Expression* expr)
    : Statement(std::move(source)), var_(std::move(var)), expr_(expr) {}

LetStatement::~LetStatement() { delete expr_; }

void LetStatement::execute(VarState& state, Program& program) const {
  int value = expr_->evaluate(state);
  state.setValue(var_, value);
}

// PrintStatement
PrintStatement::PrintStatement(std::string source, Expression* expr)
    : Statement(std::move(source)), expr_(expr) {}

PrintStatement::~PrintStatement() { delete expr_; }

void PrintStatement::execute(VarState& state, Program& program) const {
  int value = expr_->evaluate(state);
  std::cout << value << "\n";
}

// InputStatement
InputStatement::InputStatement(std::string source, std::string var)
    : Statement(std::move(source)), var_(std::move(var)) {}

void InputStatement::execute(VarState& state, Program& program) const {
  std::cout << " ? ";
  int value;
  std::cin >> value;
  state.setValue(var_, value);
}

// GotoStatement
GotoStatement::GotoStatement(std::string source, int target)
    : Statement(std::move(source)), target_(target) {}

void GotoStatement::execute(VarState& state, Program& program) const {
  program.changePC(target_);
}

// IfStatement
IfStatement::IfStatement(std::string source, Expression* left, char op,
                         Expression* right, int target)
    : Statement(std::move(source)),
      left_(left),
      right_(right),
      op_(op),
      target_(target) {}

IfStatement::~IfStatement() {
  delete left_;
  delete right_;
}

void IfStatement::execute(VarState& state, Program& program) const {
  int lhs = left_->evaluate(state);
  int rhs = right_->evaluate(state);
  bool result = false;

  switch (op_) {
    case '=':
      result = (lhs == rhs);
      break;
    case '<':
      result = (lhs < rhs);
      break;
    case '>':
      result = (lhs > rhs);
      break;
  }

  if (result) {
    program.changePC(target_);
  }
}

// RemStatement
RemStatement::RemStatement(std::string source) : Statement(std::move(source)) {}

void RemStatement::execute(VarState& state, Program& program) const {
  // REM does nothing
}

// EndStatement
EndStatement::EndStatement(std::string source) : Statement(std::move(source)) {}

void EndStatement::execute(VarState& state, Program& program) const {
  program.programEnd();
}
