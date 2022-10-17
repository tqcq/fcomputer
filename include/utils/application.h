/**
 * Created by Tqcq Ann on 10/16/22.
**/
#ifndef FCOMPUTER_INCLUDE_COMMAND_H_
#define FCOMPUTER_INCLUDE_COMMAND_H_

#include <string>

namespace tqcq {
class Application {
 public:
  virtual ~Application() {};
  /**
   *
   * @return  0 => RunSuccess, other => RunFailure
   */
  virtual int Run(const int argc, const char *argv[]) = 0;
  virtual std::string abbr() const = 0;
  virtual std::string name() const = 0;
  virtual std::string description() const = 0;
};
}

#endif //FCOMPUTER_INCLUDE_COMMAND_H_
