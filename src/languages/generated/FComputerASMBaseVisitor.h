
// Generated from FComputerASM.g4 by ANTLR 4.11.1

#pragma once


#include "antlr4-runtime.h"
#include "FComputerASMVisitor.h"


/**
 * This class provides an empty implementation of FComputerASMVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  FComputerASMBaseVisitor : public FComputerASMVisitor {
public:

  virtual std::any visitMain(FComputerASMParser::MainContext *ctx) override {
    return visitChildren(ctx);
  }


};

