#include "command.hpp"
#include <boost/filesystem.hpp>

using namespace std;
using namespace Crails;

bool Command::initialize(int argc, const char** argv)
{
  boost::program_options::options_description desc("Options");

  desc.add_options()("help,h", "produce help message");
  options_description(desc);
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), options);
  boost::program_options::notify(options);
  if (options.count("help"))
  {
    std::cout << "usage: " << argv[0] << " [options]" << std::endl << desc;
    return false;
  }
  if (with_configuration)
    configuration.initialize();
  return true;
}
