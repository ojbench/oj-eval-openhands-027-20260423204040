#include "Program.hpp"

#include "Statement.hpp"

Program::Program() : programCounter_(-1), programEnd_(false) {}

void Program::addStmt(int line, Statement* stmt) { recorder_.add(line, stmt); }

void Program::removeStmt(int line) { recorder_.remove(line); }

void Program::run() {
  // Get the first line number
  programCounter_ = recorder_.nextLine(-1);
  programEnd_ = false;

  while (programCounter_ != -1 && !programEnd_) {
    const Statement* stmt = recorder_.get(programCounter_);
    if (stmt) {
      int currentLine = programCounter_;
      // Move to next line before executing (in case of GOTO)
      programCounter_ = recorder_.nextLine(programCounter_);
      // Execute the statement
      stmt->execute(vars_, *this);
      // If PC was changed by GOTO/IF, don't override it
      if (programCounter_ == recorder_.nextLine(currentLine)) {
        // PC wasn't changed, continue normally
      }
    } else {
      break;
    }
  }

  resetAfterRun();
}

void Program::list() const { recorder_.printLines(); }

void Program::clear() {
  recorder_.clear();
  vars_.clear();
}

void Program::execute(Statement* stmt) { stmt->execute(vars_, *this); }

int Program::getPC() const noexcept { return programCounter_; }

void Program::changePC(int line) { programCounter_ = line; }

void Program::programEnd() { programEnd_ = true; }

void Program::resetAfterRun() noexcept {
  programCounter_ = -1;
  programEnd_ = false;
}