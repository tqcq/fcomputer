
// Generated from FComputerASM.g4 by ANTLR 4.11.1

#pragma once


#include "antlr4-runtime.h"




class  FComputerASMLexer : public antlr4::Lexer {
public:
  enum {
    Expression = 1, OP = 2, MUL = 3, DIV = 4, ADD = 5, SUB = 6, NUM = 7, 
    ID = 8, WS = 9
  };

  explicit FComputerASMLexer(antlr4::CharStream *input);

  ~FComputerASMLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

