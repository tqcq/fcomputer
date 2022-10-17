/**
 * Created by Tqcq Ann on 10/16/22.
**/
#ifndef FCOMPUTER_SRC_INSTRUCTION_SET_GENERATOR_INSTRUCTION_SET_GENERATOR_H_
#define FCOMPUTER_SRC_INSTRUCTION_SET_GENERATOR_INSTRUCTION_SET_GENERATOR_H_
#include <utils/cmdline.h>
#include <utils/application.h>
namespace tqcq {
class InstructionSetGenerator : public Application {
 public:
  InstructionSetGenerator();
  ~InstructionSetGenerator() override;
  int Run(const int argc, const char* argv[]) override;
  std::string abbr() const override;
  std::string name() const override;
  std::string description() const override;

 private:
  bool ParseArgs(const int argc, const char *argv[], cmdline::parser& parser);
  bool ReadFile(const std::string& filename, std::map<std::string, double>& weights);
  bool BuildHuffmanTree(const std::map<std::string, double>& weights, std::map<std::string, std::string>& operator_codes);

};
} // namespace tqcq

#endif //FCOMPUTER_SRC_INSTRUCTION_SET_GENERATOR_INSTRUCTION_SET_GENERATOR_H_
