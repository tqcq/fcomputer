/**
 * Created by Tqcq Ann on 11/16/22.
**/
#ifndef FCOMPUTER_INCLUDE_UTILS_H_
#define FCOMPUTER_INCLUDE_UTILS_H_
#include <string>

static void RemoveComment(std::string& str) {
  auto pos = str.find_first_of("#");
  if (pos != std::string::npos) {
    str = str.substr(0, pos);
  }

  pos = str.find_first_of(";");
  if (pos != std::string::npos) {
    str = str.substr(0, pos);
  }
}

static std::string UniqueLabel(const std::string& prefix) {
    static int unique_id = 0;
    return prefix + std::to_string(unique_id);
}

#endif //FCOMPUTER_INCLUDE_UTILS_H_
