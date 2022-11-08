
// Generated from FComputerASM.g4 by ANTLR 4.11.1

#pragma once


#include "antlr4-runtime.h"
#include "FComputerASMParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by FComputerASMParser.
 */
class  FComputerASMVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by FComputerASMParser.
   */
    virtual std::any visitMain(FComputerASMParser::MainContext *context) = 0;


};

