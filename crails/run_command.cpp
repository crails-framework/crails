#include <boost/process.hpp>

bool run_command(const string& command)
{
  boost::process::child process(command);

  process.wait();
  return process.exit_code() == 0;
}
