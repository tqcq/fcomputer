/**
 * Created by Tqcq Ann on 10/16/22.
**/

#include "utils/cmdline.h"
#include "instruction_set_generator.h"

#include <iostream>
#include <string>
#include <memory>
using namespace std;
using namespace tqcq;

int main(int argc, const char *argv[]) {
  cmdline::parser parser;
  parser.set_program_name("fcomputer");
  parser.add<string>("help", 'h', "Display this help message", false, "");

  std::vector<std::shared_ptr<Application>> apps = {
      std::make_shared<InstructionSetGenerator>(),
  };

  for_each(apps.begin(), apps.end(), [&parser=parser](std::shared_ptr<Application> app_ptr) {
    parser.add(app_ptr->abbr(), 0, app_ptr->description());
  });

  auto run_iter = find_if(apps.begin(), apps.end(), [&](auto app){
    for (int i = 0; i < argc; ++i) {
      if (strcmp(argv[i], app->abbr().c_str()) == 0) {
        app->Run(argc - 1, argv + 1);
        return true;
      }
    }
    return false;
  });

  if (run_iter == apps.end() && parser.exist("help")) {
    std::cerr << parser.usage() << std::endl;
    return 1;
  }

  return 0;
}