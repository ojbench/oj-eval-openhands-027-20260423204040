#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

class LetStatement : public Statement {
 public:
  LetStatement(std::string source, std::string var, Expression* expr);
  ~LetStatement();
  void execute(VarState& state, Program& program) const override;

 private:
  std::string var_;
  Expression* expr_;
};

class PrintStatement : public Statement {
 public:
  PrintStatement(std::string source, Expression* expr);
  ~PrintStatement();
  void execute(VarState& state, Program& program) const override;

 private:
  Expression* expr_;
};

class InputStatement : public Statement {
 public:
  InputStatement(std::string source, std::string var);
  void execute(VarState& state, Program& program) const override;

 private:
  std::string var_;
};

class GotoStatement : public Statement {
 public:
  GotoStatement(std::string source, int target);
  void execute(VarState& state, Program& program) const override;

 private:
  int target_;
};

class IfStatement : public Statement {
 public:
  IfStatement(std::string source, Expression* left, char op, Expression* right,
              int target);
  ~IfStatement();
  void execute(VarState& state, Program& program) const override;

 private:
  Expression* left_;
  Expression* right_;
  char op_;
  int target_;
};

class RemStatement : public Statement {
 public:
  RemStatement(std::string source);
  void execute(VarState& state, Program& program) const override;
};

class EndStatement : public Statement {
 public:
  EndStatement(std::string source);
  void execute(VarState& state, Program& program) const override;
};
