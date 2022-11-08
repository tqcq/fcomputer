
// Generated from FComputerASM.g4 by ANTLR 4.11.1

#pragma once


#include "antlr4-runtime.h"




class  FComputerASMParser : public antlr4::Parser {
public:
  enum {
    Expression = 1, OP = 2, MUL = 3, DIV = 4, ADD = 5, SUB = 6, NUM = 7, 
    ID = 8, WS = 9
  };

  enum {
    RuleMain = 0
  };

  explicit FComputerASMParser(antlr4::TokenStream *input);

  FComputerASMParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~FComputerASMParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class MainContext; 

  class  MainContext : public antlr4::ParserRuleContext {
  public:
    MainContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MainContext* main();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

