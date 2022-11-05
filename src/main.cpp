#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>

using namespace std;
bool debug_flag = false;
int ret_id = 0;

struct Line {
  string opname = "nop";
  bool is_label = false;
  string label;

  bool is_jmp_label = false;
  string jump_label;

  string raw_line;

  int pc;
  std::map<string, int> signals;

  int min_cycle = 100;
  vector<int> prev;
  vector<int> next;
};

map<string, int> label_to_pc;
vector<Line> program;

/**
 * P -> PC
 * F -> opcode
 * K -> dst reg
 * M -> operand reg1
 * N -> operand reg1
 * X -> M == -1, use this immu
 * Y -> N == -2, use this immu
 * L,E,G -> for jmp
 * I -> I == 0, jump base pc; I == -1, jump to absolute address
 */

const char all_signals[] = "PFABCXYZLEGI";

vector<Line> ParseLabel(vector<string> args, int &pc);
vector<Line> ParseNop(vector<string> args, int &pc);

vector<Line> ParseJump(vector<string> args, int &pc);
vector<Line> ParseMov(vector<string> args, int &pc);
vector<Line> ParseCalc(vector<string> args, int &pc);
vector<Line> ParseInc(vector<string> args, int &pc);
vector<Line> ParseDec(vector<string> args, int &pc);
vector<Line> ParseCmp(vector<string> args, int &pc);
vector<Line> ParseLDR(vector<string> args, int &pc);
vector<Line> ParseSTR(vector<string> args, int &pc);
vector<Line> ParsePush(vector<string> args, int &pc);
vector<Line> ParsePop(vector<string> args, int &pc);
vector<Line> ParseCall(vector<string> args, int &pc);
vector<Line> ParseRet(vector<string> args, int &pc);

void OptimizeProgram(vector<Line> &program);
void ConvertProgramToCSV(vector<Line> program);

// L -> 1000, E, 1001, G -> 1002
// MemWrite -> 1003, MemRead -> 1004
#define LESS_IDX (1000)
#define EQUAL_IDX (1001)
#define GREATER_IDX (1002)
#define MEM_WRITE_IDX (1003)
#define MEM_READ_IDX (1004)

std::map<int, set<string>> id_to_reg = {
    {0, {"ax"}},
    {1, {"bx"}},
    {2, {"cx"}},
    {3, {"dx"}},

    {100, {"bp"}}, // signal-water
    {101, {"sp"}}, // signal-oil
    {102, {"pc"}}, // signal-P
    // {110, {"x"}}, imm1
    // {111, {"y"}}, imm2
    // {112, {"z"}}, imm3
    {120, {"rand", "random"}}, // random reg
    {121, {"tmp_pc"}} // random reg
};

std::map<vector<Line> (*)(vector<string>, int &), set<string>> handlers = {
    {ParseNop, {"nop"}},
    {ParseJump, {"jmp.*", "jz.*", "je.*", "jne.*", "jg.*", "jl.*", "jnz.*", "jle.*", "jge.*"}},
    {ParseMov, {"mov.*"}},
    {ParseCalc, {"add.*", "sub.*", "mul.*", "div.*", "mod.*", "and.*", "or.*", "xor.*"}},
    {ParseInc, {"inc.*"}},
    {ParseDec, {"dec.*"}},
    {ParseCmp, {"cmp.*", "test.*"}},
    {ParseLDR, {"ldr.*", "ldi.*", "ld.*"}},
    {ParseSTR, {"str.*", "ldi.*", "ld.*"}},
    {ParsePush, {"push.*"}},
    {ParsePop, {"pop.*"}},
    {ParseCall, {"call.*"}},
    {ParseRet, {"ret.*"}},
    {ParseLabel, {"[a-zA-Z_][a-zA-Z0-9_]+:"}},
};

vector<string> Split(string args) {
  vector<string> result;
  string tmp;
  for (int i = 0; i < args.size(); ++i) {
    if (isspace(args[i]) || args[i] == ',') {
      if (!tmp.empty()) {
        result.push_back(tmp);
        tmp.clear();
      }
    } else {
      tmp.push_back(tolower(args[i]));
    }
  }
  if (!tmp.empty())
    result.emplace_back(tmp);
  return result;
}

static bool IsImmu(const string &str) {
  return regex_match(str, regex("\\d+"));
}

static bool Match(const string &str, const string &reg) {
  return regex_match(str, regex(reg, regex::icase));
}

static bool IsMemAddr(const string &str) {
  return str.find("(") != string::npos && str.find(")") != string::npos;
}

static int ToInt(const string &str) {
  return strtol(str.c_str(), NULL, 10);
}

static int reg_to_id(string reg_name) {
  transform(reg_name.begin(), reg_name.end(), reg_name.begin(), [](char c){return tolower(c); });
  auto iter = std::find_if(id_to_reg.begin(), id_to_reg.end(), [&](pair<int, set<string>> p) {
    return p.second.find(reg_name) != p.second.end();
  });
  return iter == id_to_reg.end() ? -1 : iter->first;
}

bool ProcessLine(string str, int &pc) {
  auto args = Split(str);
  if (args.empty() || args[0][0] == ';')
    return true;
  bool match_flag = false;
  for (auto pair : handlers) {
    auto handle = pair.first;
    for (auto key : pair.second) {
      if (regex_match(args[0], regex(key, regex::icase))) {
        match_flag = true;
        auto lines = handle(args, pc);
        for (int i = 0; i < lines.size(); i++) {
          if (lines[i].raw_line.empty()) {
            if (i == 0)
              lines[i].raw_line = str;
            else
              lines[i].raw_line = "     -> " + str;
          }
        }
        program.insert(program.end(), lines.begin(), lines.end());
        break;
      }
    }
    if (match_flag)
      break;
  }
  if (!match_flag) {
    cout << "Error: unknown " << str << endl;
    return false;
  }
  return true;
}

static string GetIMMUSignal(const string &s) {
  if (s == "A")
    return "X";
  if (s == "B")
    return "Y";
  if (s == "C")
    return "Z";
  return "unknown";
}

static int GetIMMUIdx(const string& s) {
  if (s == "A")
    return 110;
  if (s == "B")
    return 111;
  if (s == "C")
    return 112;
  return -111;
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    debug_flag = true;
  }

  for (int i = 0; i < 32; ++i) {
    id_to_reg[i].insert(string("r") + to_string(i));
  }

  string str;

  int pc = 1;

  ProcessLine("mov sp,2048", pc);
  while (getline(cin, str)) {
    if (!ProcessLine(str, pc)) {
      return -1;
    }
  }
  ProcessLine("_system_end_label:", pc);
  ProcessLine("jmp _system_end_label", pc);

  OptimizeProgram(program);
  for (int i = 0; i < program.size(); ++i) {
    auto &line = program[i];
    if (line.is_jmp_label) {
      int label_address = label_to_pc[line.jump_label];
      if (label_address) {
        if (line.signals["F"] == 2) {
          line.signals["B"] = GetIMMUIdx("B");
          line.signals[GetIMMUSignal("B")] = label_address;
        } else if (line.signals["F"] == 3) {
          line.signals["A"] = GetIMMUIdx("A");
          line.signals[GetIMMUSignal("A")] = label_address;
        }
      } else {
        cout << "Can't find jump label " << line.jump_label << endl;
      }
    }
    // Display All Program(Replace label to addr)
    if (debug_flag) {
      str = line.raw_line;
      if (line.is_jmp_label) {
        if (str.find(line.jump_label) != string::npos) {
          str.replace(str.find(line.jump_label),
                      line.jump_label.length(),
                      to_string(label_to_pc[line.jump_label]));
        }
      }
      //  if (!line.is_label)
      cout << line.pc << "\t: " << str << endl;
    }
  }

  ConvertProgramToCSV(program);

  return 0;
}

vector<Line> ParseLabel(vector<string> args, int &pc) {
  if (label_to_pc.find(args[0]) == label_to_pc.end()) {
    label_to_pc[args[0].substr(0, args[0].length() - 1)] = pc;
  }
  Line line;
  line.pc = pc;
  line.is_label = true;
  line.label = args[0].substr(0, args[0].length() - 1);
  line.min_cycle = 0;
  line.opname = "label: " + line.label;
  return {line};
}

vector<Line> ParseNop(vector<string> args, int &pc) {
  Line line;
  line.pc = pc;
  pc += 1;
  line.min_cycle = 1;
  line.opname = "label: Nop";
  return {line};
}

vector<Line> ParseJump(vector<string> args, int &pc) {
  Line line;
  line.pc = pc;
  line.min_cycle = 1;
  pc += 1;
  line.signals["F"] = 2;
  line.opname = "label: " + args[0];

  // I == 0 -> pc base
  // I == -1 -> real address
  line.signals["I"] = -1;

  if (Match(args[0], "jmp")) {
    line.signals["L"] = 2;
    line.signals["E"] = 2;
    line.signals["G"] = 2;
  } else if (Match(args[0], "jz") || Match(args[0], "je")) {
    line.signals["E"] = 1;
    line.prev.push_back(EQUAL_IDX);
  } else if (Match(args[0], "jl")) {
    line.signals["L"] = 1;
    line.prev.push_back(LESS_IDX);
  } else if (Match(args[0], "jg")) {
    line.signals["G"] = 1;
    line.prev.push_back(GREATER_IDX);
  } else if (Match(args[0], "jnz") || Match(args[0], "jne")) {
    line.signals["L"] = 1;
    line.signals["G"] = 1;
    line.prev.push_back(LESS_IDX);
    line.prev.push_back(GREATER_IDX);
  } else if (Match(args[0], "jle")) {
    line.signals["L"] = 1;
    line.signals["E"] = 1;
    line.prev.push_back(LESS_IDX);
    line.prev.push_back(EQUAL_IDX);
  } else if (Match(args[0], "jge")) {
    line.signals["G"] = 1;
    line.signals["E"] = 1;
    line.prev.push_back(EQUAL_IDX);
    line.prev.push_back(GREATER_IDX);
  }

  if (IsImmu(args[1])) {
    line.signals["B"] = GetIMMUIdx("B");
    line.signals[GetIMMUSignal("B")] = ToInt(args[1]);
  } else if (reg_to_id(args[1]) >= 0) {
    line.signals["B"] = reg_to_id(args[1]);
    line.prev.push_back(reg_to_id(args[1]));
  } else {
    line.is_jmp_label = true;
    line.jump_label = args[1];
  };

  line.signals["C"] = -1;

  return {line};
}
vector<Line> ParseMov(vector<string> args, int &pc) {
  Line line;
  line.pc = pc;
  line.min_cycle = 6;
  line.opname = "label: mov";
  pc += 6;
  line.signals["F"] = 8;
  line.signals["A"] = reg_to_id(args[1]);
  line.next.push_back(reg_to_id(args[1]));

  if (IsImmu(args[2])) {
    line.signals["B"] = GetIMMUIdx("B");
    line.signals[GetIMMUSignal("B")] = ToInt(args[2]);
  } else {
    line.signals["B"] = reg_to_id(args[2]);
    line.prev.push_back(reg_to_id(args[2]));
  }

  line.signals["C"] = -1;


  return {line};
}
vector<Line> ParseCalc(vector<string> args, int &pc) {
  Line line;
  line.pc = pc;
  line.min_cycle = 6;
  line.opname = "label: " + args[0];
  pc += 6;

  line.signals["A"] = reg_to_id(args[1]);
  line.next.push_back(reg_to_id(args[1]));

  if (IsImmu(args[2])) {
    line.signals["B"] = GetIMMUIdx("B");
    line.signals[GetIMMUSignal("B")] = ToInt(args[2]);
  } else {
    line.signals["B"] = reg_to_id(args[2]);
    line.prev.push_back(reg_to_id(args[2]));
  }

  if (IsImmu(args[3])) {
    line.signals["C"] = GetIMMUIdx("C");
    line.signals[GetIMMUSignal("C")] = ToInt(args[3]);
  } else {
    line.signals["C"] = reg_to_id(args[3]);
    line.prev.push_back(reg_to_id(args[3]));
  }

  if (Match(args[0], "add")) {
    line.signals["F"] = 8;
  } else if (Match(args[0], "sub")) {
    line.signals["F"] = 9;
  } else if (Match(args[0], "mul")) {
    line.signals["F"] = 10;
  } else if (Match(args[0], "div")) {
    line.signals["F"] = 11;
  } else if (Match(args[0], "mod")) {
    line.signals["F"] = 12;
  } else if (Match(args[0], "and")) {
    line.signals["F"] = 13;
  } else if (Match(args[0], "or")) {
    line.signals["F"] = 14;
  } else if (Match(args[0], "xor")) {
    line.signals["F"] = 15;
  }

  return {line};
}
vector<Line> ParseInc(vector<string> args, int &pc) {
  vector<string> new_args = {
      "add",
      args[1],
      args[1],
      "1"};

  return ParseCalc(new_args, pc);
}

vector<Line> ParseDec(vector<string> args, int &pc) {
  vector<string> new_args = {
      "sub",
      args[1],
      args[1],
      "1"};

  return ParseCalc(new_args, pc);
}

vector<Line> ParseCmp(vector<string> args, int &pc) {
  Line line;
  line.pc = pc;
  pc += 8;
  line.min_cycle = 8;
  line.opname = args[0];
  line.signals["F"] = 1;
  line.next.push_back(LESS_IDX);
  line.next.push_back(EQUAL_IDX);
  line.next.push_back(GREATER_IDX);

  if (IsImmu(args[1])) {
    line.signals["B"] = GetIMMUIdx("B");
    line.signals[GetIMMUSignal("B")] = ToInt(args[1]);
  } else {
    line.signals["B"] = reg_to_id(args[1]);
    line.prev.push_back(reg_to_id(args[1]));
  }

  if (IsImmu(args[2])) {
    line.signals["C"] = GetIMMUIdx("C");
    line.signals[GetIMMUSignal("C")] = ToInt(args[2]);
  } else {
    line.signals["C"] = reg_to_id(args[2]);
    line.prev.push_back(reg_to_id(args[2]));
  }

  return {line};
}
vector<Line> ParseLDR(vector<string> args, int &pc) {
  Line line;
  line.pc = pc;
  line.min_cycle = 10;
  line.opname = args[0];
  pc += 10;
  line.signals["F"] = 4;
  line.signals["A"] = reg_to_id(args[1]);
  line.prev.push_back(MEM_WRITE_IDX);
  line.next.push_back(reg_to_id(args[1]));

  string str = args[2].substr(1, args[2].length() - 2);
  if (IsImmu(str)) {
    line.signals["B"] = GetIMMUIdx("B");
    line.signals[GetIMMUSignal("B")] = ToInt(str);
  } else {
    line.signals["B"] = reg_to_id(str);
    line.prev.push_back(reg_to_id(str));
  }

  line.signals["C"] = -1;

  return {line};
}

vector<Line> ParseSTR(vector<string> args, int &pc) {
  Line line;
  line.pc = pc;
  line.min_cycle = 6;
  line.opname = args[0];
  pc += 6;
  line.signals["F"] = 3;
  line.next.push_back(MEM_WRITE_IDX);

  string str = args[1].substr(1, args[1].length() - 2);
  if (IsImmu(str)) {
    line.signals["B"] = GetIMMUIdx("B");
    line.signals[GetIMMUSignal("B")] = ToInt(str);
  } else {
    line.signals["B"] = reg_to_id(str);
    line.prev.push_back(reg_to_id(str));
  }

  if (IsImmu(args[2])) {
    line.signals["A"] = GetIMMUIdx("A");
    line.signals[GetIMMUSignal("A")] = ToInt(args[2]);
  } else if (reg_to_id(args[2]) >= 0) {
    line.signals["A"] = reg_to_id(args[2]);
    line.prev.push_back(reg_to_id(args[2]));
  } else {
    line.jump_label = args[2];
    line.is_jmp_label = true;
  }
  line.signals["C"] = -1;

  return {line};
}

vector<Line> ParsePush(vector<string> args, int &pc) {
  vector<Line> lines = {
      ParseDec({"dec", "sp"}, pc)[0],
      ParseSTR({"str", "[sp]", args[1]}, pc)[0],
  };
  lines[0].raw_line = "dec sp";
  lines[1].raw_line = "str [sp], " + args[1];
  return lines;
}

vector<Line> ParsePop(vector<string> args, int &pc) {
  vector<Line> lines = {
      ParseLDR({"ldr", args[1], "[sp]"}, pc)[0],
      ParseInc({"inc", "sp"}, pc)[0],
  };
  lines[0].raw_line = "ldr " + args[1] + ",[sp]";
  lines[1].raw_line = "inc sp";
  return lines;
}

vector<Line> ParseCall(vector<string> args, int &pc) {
  vector<Line> lines;
  string ret_label = "_system_ret_label_" + to_string(ret_id);
  ret_id++;

  auto push_lines = ParsePush({"push", ret_label}, pc);
  auto jump_lines = ParseJump({"jmp", args[1]}, pc);
  auto label_lines = ParseLabel({ret_label + ":"}, pc);
  // auto push_lines = ParsePush({"push", "pc"}, pc);
  // auto jump_lines = ParseJump({"jmp", args[1]}, pc);

  push_lines[0].raw_line = "push " + ret_label + "\t\t -> " + push_lines[0].raw_line;
  jump_lines[0].raw_line = "jmp " + args[1];
  label_lines[0].raw_line = ret_label + ":";

  for (auto line : push_lines)
    lines.emplace_back(line);

  for (auto line : jump_lines)
    lines.emplace_back(line);

  for (auto line : label_lines)
    lines.emplace_back(line);

  return lines;
}

vector<Line> ParseRet(vector<string> args, int &pc) {
  vector<Line> lines;
  auto pop_lines = ParsePop({"pop", "tmp_pc"}, pc);
  auto jump_lines = ParseJump({"jmp", "tmp_pc"}, pc);
  pop_lines[0].raw_line = string("ret") + "\t\t -> " + pop_lines[0].raw_line;
  pop_lines[1].raw_line = "\t\t -> " + pop_lines[1].raw_line;
  jump_lines[0].raw_line = "\t\t -> jmp tmp_pc";

  for (auto line : pop_lines)
    lines.emplace_back(line);

  for (auto line : jump_lines)
    lines.emplace_back(line);

  return lines;
}

void OptimizeProgram(vector<Line> &program) {
  for (auto &line : program) {
    sort(line.prev.begin(), line.prev.end());
    sort(line.next.begin(), line.next.end());
  }

  function<void(const vector<Line> &)> DisplayProgram = [](const vector<Line> &p) {
    if (!debug_flag) {
      return;
    }

    for (int i = 0; i < p.size(); i++) {
      cout << p[i].pc << " ==> " << p[i].min_cycle << " " << p[i].raw_line;
      if (!p[i].is_label) {
        cout << " : \t\t\t[";
        for (auto prev : p[i].prev)
          cout << prev << " ";
        cout << " ], [ ";
        for (auto next : p[i].next)
          cout << next << " ";
        cout << "] " << endl;
      } else {
        cout << endl;
      }
    }
    cout << endl;
  };

  DisplayProgram(program);

  for (int i = 1; i < program.size(); i++) {
    if (program[i].is_label)
      continue;

    int j = i - 1;
    while (j >= 0 && program[j].is_label) --j;
    if (j < 0)
      continue;
    program[i].pc = program[j].pc + 1;

    int diff = 0;
    for (; j >= 0; --j) {
      if (program[j].is_label)
        continue;
      diff += 1;

      if (diff > 10)
        continue;

      auto &s1 = program[j].next;
      auto &s2 = program[i].prev;
      bool need_check = false;
      int offset = 0;

      for (auto s1_item : s1) {
        for (auto s2_item : s2) {
          if (s1_item == s2_item) {
            need_check = true;
            break;
          }
        }
      }

      // jump
      if (program[i].signals["F"] == 2) {
        if (program[j].signals["F"] != 1 && !need_check) {
          offset = 7;
        }
        need_check = true;
      }
      if (need_check) {
        while (program[i].pc - program[j].pc < program[j].min_cycle - offset) {
          program[i].pc++;
        }
      }
    }
  }

  for (int i = 1; i < program.size(); ++i) {
    if (!program[i].is_label) {
      for (int j = i - 1; j >= 0 && program[j].is_label; --j) {
        program[j].pc = program[i].pc;
        label_to_pc[program[j].label] = program[i].pc;
        // cout << "update label " << program[j].label << " to " << program[i].pc << endl;
      }
    }
  }

  DisplayProgram(program);
}

void ConvertProgramToCSV(vector<Line> program) {
  for (int i = 0; all_signals[i]; ++i) {
    cout << all_signals[i];
    if (all_signals[i + 1])
      cout << ",";
    cout << " ";
  }
  cout << endl;

  for (auto line : program) {
    if (line.is_label) {
      continue;
    }

    cout << line.pc;

    for (int i = 0; all_signals[i]; ++i) {
      string s;
      s.push_back(all_signals[i]);
      if (line.signals.find(s) != line.signals.end()) {
        cout << line.signals[s];
      }
      if (all_signals[i + 1])
        cout << ",";
      cout << " ";
    }
    cout << endl;
  }
}
