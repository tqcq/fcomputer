
// Generated from FComputerASM.g4 by ANTLR 4.11.1


#include "FComputerASMLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct FComputerASMLexerStaticData final {
  FComputerASMLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  FComputerASMLexerStaticData(const FComputerASMLexerStaticData&) = delete;
  FComputerASMLexerStaticData(FComputerASMLexerStaticData&&) = delete;
  FComputerASMLexerStaticData& operator=(const FComputerASMLexerStaticData&) = delete;
  FComputerASMLexerStaticData& operator=(FComputerASMLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag fcomputerasmlexerLexerOnceFlag;
FComputerASMLexerStaticData *fcomputerasmlexerLexerStaticData = nullptr;

void fcomputerasmlexerLexerInitialize() {
  assert(fcomputerasmlexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<FComputerASMLexerStaticData>(
    std::vector<std::string>{
      "Expression", "OP", "MUL", "DIV", "ADD", "SUB", "NUM", "ID", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "", "", "'*'", "'/'", "'+'", "'-'"
    },
    std::vector<std::string>{
      "", "Expression", "OP", "MUL", "DIV", "ADD", "SUB", "NUM", "ID", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,9,77,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,
  	2,7,7,7,2,8,7,8,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
  	0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3,0,43,8,0,1,1,1,1,1,1,1,1,3,1,
  	49,8,1,1,2,1,2,1,3,1,3,1,4,1,4,1,5,1,5,1,6,4,6,60,8,6,11,6,12,6,61,1,
  	7,1,7,5,7,66,8,7,10,7,12,7,69,9,7,1,8,4,8,72,8,8,11,8,12,8,73,1,8,1,8,
  	0,0,9,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,1,0,4,1,0,48,57,3,0,65,
  	90,95,95,97,122,4,0,48,57,65,90,95,95,97,122,3,0,9,10,13,13,32,32,87,
  	0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,
  	0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,1,42,1,0,0,0,3,48,1,0,0,0,
  	5,50,1,0,0,0,7,52,1,0,0,0,9,54,1,0,0,0,11,56,1,0,0,0,13,59,1,0,0,0,15,
  	63,1,0,0,0,17,71,1,0,0,0,19,20,5,40,0,0,20,21,3,1,0,0,21,22,5,41,0,0,
  	22,43,1,0,0,0,23,24,3,13,6,0,24,25,3,3,1,0,25,26,3,1,0,0,26,43,1,0,0,
  	0,27,28,3,13,6,0,28,29,3,3,1,0,29,30,5,40,0,0,30,31,3,1,0,0,31,32,5,41,
  	0,0,32,43,1,0,0,0,33,34,3,13,6,0,34,35,3,3,1,0,35,36,3,13,6,0,36,43,1,
  	0,0,0,37,38,5,40,0,0,38,39,3,13,6,0,39,40,5,41,0,0,40,43,1,0,0,0,41,43,
  	3,13,6,0,42,19,1,0,0,0,42,23,1,0,0,0,42,27,1,0,0,0,42,33,1,0,0,0,42,37,
  	1,0,0,0,42,41,1,0,0,0,43,2,1,0,0,0,44,49,3,5,2,0,45,49,3,7,3,0,46,49,
  	3,9,4,0,47,49,3,11,5,0,48,44,1,0,0,0,48,45,1,0,0,0,48,46,1,0,0,0,48,47,
  	1,0,0,0,49,4,1,0,0,0,50,51,5,42,0,0,51,6,1,0,0,0,52,53,5,47,0,0,53,8,
  	1,0,0,0,54,55,5,43,0,0,55,10,1,0,0,0,56,57,5,45,0,0,57,12,1,0,0,0,58,
  	60,7,0,0,0,59,58,1,0,0,0,60,61,1,0,0,0,61,59,1,0,0,0,61,62,1,0,0,0,62,
  	14,1,0,0,0,63,67,7,1,0,0,64,66,7,2,0,0,65,64,1,0,0,0,66,69,1,0,0,0,67,
  	65,1,0,0,0,67,68,1,0,0,0,68,16,1,0,0,0,69,67,1,0,0,0,70,72,7,3,0,0,71,
  	70,1,0,0,0,72,73,1,0,0,0,73,71,1,0,0,0,73,74,1,0,0,0,74,75,1,0,0,0,75,
  	76,6,8,0,0,76,18,1,0,0,0,6,0,42,48,61,67,73,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  fcomputerasmlexerLexerStaticData = staticData.release();
}

}

FComputerASMLexer::FComputerASMLexer(CharStream *input) : Lexer(input) {
  FComputerASMLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *fcomputerasmlexerLexerStaticData->atn, fcomputerasmlexerLexerStaticData->decisionToDFA, fcomputerasmlexerLexerStaticData->sharedContextCache);
}

FComputerASMLexer::~FComputerASMLexer() {
  delete _interpreter;
}

std::string FComputerASMLexer::getGrammarFileName() const {
  return "FComputerASM.g4";
}

const std::vector<std::string>& FComputerASMLexer::getRuleNames() const {
  return fcomputerasmlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& FComputerASMLexer::getChannelNames() const {
  return fcomputerasmlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& FComputerASMLexer::getModeNames() const {
  return fcomputerasmlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& FComputerASMLexer::getVocabulary() const {
  return fcomputerasmlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView FComputerASMLexer::getSerializedATN() const {
  return fcomputerasmlexerLexerStaticData->serializedATN;
}

const atn::ATN& FComputerASMLexer::getATN() const {
  return *fcomputerasmlexerLexerStaticData->atn;
}




void FComputerASMLexer::initialize() {
  ::antlr4::internal::call_once(fcomputerasmlexerLexerOnceFlag, fcomputerasmlexerLexerInitialize);
}
