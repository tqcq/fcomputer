
// Generated from FComputerASM.g4 by ANTLR 4.11.1


#include "FComputerASMListener.h"
#include "FComputerASMVisitor.h"

#include "FComputerASMParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct FComputerASMParserStaticData final {
  FComputerASMParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  FComputerASMParserStaticData(const FComputerASMParserStaticData&) = delete;
  FComputerASMParserStaticData(FComputerASMParserStaticData&&) = delete;
  FComputerASMParserStaticData& operator=(const FComputerASMParserStaticData&) = delete;
  FComputerASMParserStaticData& operator=(FComputerASMParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag fcomputerasmParserOnceFlag;
FComputerASMParserStaticData *fcomputerasmParserStaticData = nullptr;

void fcomputerasmParserInitialize() {
  assert(fcomputerasmParserStaticData == nullptr);
  auto staticData = std::make_unique<FComputerASMParserStaticData>(
    std::vector<std::string>{
      "main"
    },
    std::vector<std::string>{
      "", "", "", "'*'", "'/'", "'+'", "'-'"
    },
    std::vector<std::string>{
      "", "Expression", "OP", "MUL", "DIV", "ADD", "SUB", "NUM", "ID", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,9,5,2,0,7,0,1,0,1,0,1,0,0,0,1,0,0,0,3,0,2,1,0,0,0,2,3,5,1,0,0,3,1,
  	1,0,0,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  fcomputerasmParserStaticData = staticData.release();
}

}

FComputerASMParser::FComputerASMParser(TokenStream *input) : FComputerASMParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

FComputerASMParser::FComputerASMParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  FComputerASMParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *fcomputerasmParserStaticData->atn, fcomputerasmParserStaticData->decisionToDFA, fcomputerasmParserStaticData->sharedContextCache, options);
}

FComputerASMParser::~FComputerASMParser() {
  delete _interpreter;
}

const atn::ATN& FComputerASMParser::getATN() const {
  return *fcomputerasmParserStaticData->atn;
}

std::string FComputerASMParser::getGrammarFileName() const {
  return "FComputerASM.g4";
}

const std::vector<std::string>& FComputerASMParser::getRuleNames() const {
  return fcomputerasmParserStaticData->ruleNames;
}

const dfa::Vocabulary& FComputerASMParser::getVocabulary() const {
  return fcomputerasmParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView FComputerASMParser::getSerializedATN() const {
  return fcomputerasmParserStaticData->serializedATN;
}


//----------------- MainContext ------------------------------------------------------------------

FComputerASMParser::MainContext::MainContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FComputerASMParser::MainContext::Expression() {
  return getToken(FComputerASMParser::Expression, 0);
}


size_t FComputerASMParser::MainContext::getRuleIndex() const {
  return FComputerASMParser::RuleMain;
}

void FComputerASMParser::MainContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FComputerASMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMain(this);
}

void FComputerASMParser::MainContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<FComputerASMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMain(this);
}


std::any FComputerASMParser::MainContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FComputerASMVisitor*>(visitor))
    return parserVisitor->visitMain(this);
  else
    return visitor->visitChildren(this);
}

FComputerASMParser::MainContext* FComputerASMParser::main() {
  MainContext *_localctx = _tracker.createInstance<MainContext>(_ctx, getState());
  enterRule(_localctx, 0, FComputerASMParser::RuleMain);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2);
    match(FComputerASMParser::Expression);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void FComputerASMParser::initialize() {
  ::antlr4::internal::call_once(fcomputerasmParserOnceFlag, fcomputerasmParserInitialize);
}
