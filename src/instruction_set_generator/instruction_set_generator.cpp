/**
 * Created by Tqcq Ann on 10/16/22.
**/
#include "instruction_set_generator.h"
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
using namespace std;

namespace tqcq {

InstructionSetGenerator::InstructionSetGenerator() {
}

InstructionSetGenerator::~InstructionSetGenerator() {
}

int InstructionSetGenerator::Run(const int argc, const char *argv[]) {
  cmdline::parser parser;
  if (!ParseArgs(argc, argv, parser)) {
    return 1;
  }

  // opcode with weights
  std::map<std::string, double> weights;
  if (!ReadFile(parser.get<string>("file"), weights)) {
    return 1;
  }

  std::map<std::string, std::string> operator_codes;
  if (!BuildHuffmanTree(weights, operator_codes)) {
    return 1;
  }

  for_each(operator_codes.begin(), operator_codes.end(), [&](auto item){
    std::cout << setw(10) << left << item.first
              << setw(20) << left << item.second
              << weights[item.first] << std::endl;
  });
  return 0;
}

bool InstructionSetGenerator::ParseArgs(const int argc, const char *argv[], cmdline::parser& parser) {
  const char app_name[] = "InstructionSetGenerator";
  parser.set_program_name(app_name);
  parser.add<string>("help", 'h', "get help for InstructionSetGenerator", false);
  parser.add<string>("file", 'f', "specific opcode weights file", true);

  bool parse_ok = parser.parse(argc, argv);

  if (argc == 1 || parser.exist("help")) {
    std::cerr << parser.usage() << "\n" << std::endl;
    return false;
  }

  if (!parse_ok) {
    std::cerr << parser.error_full() << "\n" <<std::endl;
    return false;
  }


  return true;
}

string InstructionSetGenerator::abbr() const {
  return "isg";
}
string InstructionSetGenerator::name() const {
  return "Instruction Set Generator";
}
string InstructionSetGenerator::description() const {
  return name();
}
bool InstructionSetGenerator::ReadFile(const string &filename, map<std::string, double> &weights) {
  ifstream ifs(filename);
  if (!ifs.is_open()) {
    std::cerr << "Error: open file failed. " << filename << std::endl;
    return false;
  }

  std::string line;
  std::string op_name;
  double op_weight;

  weights.clear();
  while (getline(ifs, line)) {
    istringstream iss(line);
    if (!(iss >> op_name >> op_weight)) {
      // std::cerr << "Error: must contain operator and weight. >>> " << line << " <<<" << std::endl;
      // return false;
    }
    weights[op_name] = op_weight;
  }

  if (weights.empty()) {
    std::cerr << "Error: must have at least one weight." << filename << std::endl;
    return false;
  }

  return true;
}

bool InstructionSetGenerator::BuildHuffmanTree(const map<std::string, double> &weights,
                                               map<std::string, std::string> &operator_codes) {
  using T = std::pair<double, int>;
  // id to opcode's name
  std::map<int, std::string> id_name_table;
  // std::map<std::string, int> name_id_table;
  // sort by inc
  std::priority_queue<T, vector<T>, greater<T>> queue;

  // 0 -> left child, 1 -> right child
  std::vector<std::vector<int>> tree(2 * weights.size(), std::vector<int>(2, -1));

  // sort by weight
  int id = 0;
  for_each(weights.begin(), weights.end(), [&](auto item){
    queue.push(std::make_pair(item.second, id));

    id_name_table[id] = item.first;
    // name_id_table[iter->first] = id;
    id = id + 1;
  });

  // build tree
  while (queue.size() >= 2) {
    auto x = queue.top();
    queue.pop();
    auto y = queue.top();
    queue.pop();

    int new_id = id++;
    int new_weight = x.first + y.first;
    queue.push(std::make_pair(new_weight, new_id));

    tree[new_id][0] = x.second;
    tree[new_id][1] = y.second;
  }

  // encode <id, encode_str>
  std::queue<std::pair<int, std::string>> encode_queue;
  auto root = queue.top();
  encode_queue.push(std::make_pair(root.second, "0"));
  while (!encode_queue.empty()) {
    auto cur_node = encode_queue.front();
    auto cur_node_code = cur_node.second;
    encode_queue.pop();

    int cur_node_id = cur_node.first;
    for (int i = 0; i < 2; ++i) {
      int child_node_id = tree[cur_node_id][i];
      if (child_node_id != -1) {
        encode_queue.push(std::make_pair(child_node_id, cur_node_code + std::to_string(i)));
      }
    }

    // is leaf node
    if (cur_node_id < weights.size()) {
      operator_codes[id_name_table[cur_node_id]] = cur_node_code;
    }
  }

  return true;
}

} // namespace tqcq
