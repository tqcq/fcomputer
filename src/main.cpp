#include <iostream>
#include "antlr4-runtime.h"
#include "FComputerASMLexer.h"
#include "FComputerASMBaseVisitor.h"
#include "FComputerASMBaseListener.h"
using namespace antlr4;

class TreeShapeListener : public FComputerASMBaseListener {
 public:
  void exitMain(FComputerASMParser::MainContext *context) override {
    FComputerASMBaseListener::exitMain(context);
  }
};

class Visitor : public FComputerASMBaseVisitor {
 public:
  std::any visitMain(FComputerASMParser::MainContext *ctx) override {
    if (ctx->Expression() != nullptr) {
    }
    std::cout << "visitMain: " << ctx->getText() << std::endl;
    return visitChildren(ctx);
  }
};

int main(int argc, char* argv[]) {
  std::ifstream stream;
  stream.open(argv[1]);
  ANTLRInputStream input(stream);
  FComputerASMLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  FComputerASMParser parser(&tokens);

  tree::ParseTree *tree = parser.main();
  Visitor visitor;
  // TreeShapeListener listener;
  // tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
  visitor.visit(tree);
  auto s = tree->toStringTree(&parser);
  std::cout << s << std::endl;
  return 0;
}