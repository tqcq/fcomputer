
// Generated from FComputerASM.g4 by ANTLR 4.11.1

#pragma once


#include "antlr4-runtime.h"
#include "FComputerASMParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by FComputerASMParser.
 */
class  FComputerASMListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterMain(FComputerASMParser::MainContext *ctx) = 0;
  virtual void exitMain(FComputerASMParser::MainContext *ctx) = 0;


};

