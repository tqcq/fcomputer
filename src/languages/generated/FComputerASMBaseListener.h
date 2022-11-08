
// Generated from FComputerASM.g4 by ANTLR 4.11.1

#pragma once


#include "antlr4-runtime.h"
#include "FComputerASMListener.h"


/**
 * This class provides an empty implementation of FComputerASMListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  FComputerASMBaseListener : public FComputerASMListener {
public:

  virtual void enterMain(FComputerASMParser::MainContext * /*ctx*/) override { }
  virtual void exitMain(FComputerASMParser::MainContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

