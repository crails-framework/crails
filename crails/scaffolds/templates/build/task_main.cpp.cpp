#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldsTaskMainCpp : public Crails::Template
{
public:
  ScaffoldsTaskMainCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#include <boost/program_options.hpp>\n#include <iostream>\n\nusing namespace std;\n\nint main(int argc, char** argv)\n{\n  boost::program_options::options_description options_description(\"Options\");\n  boost::program_options::variables_map options;\n\n  options_description.add_options()(\"help,h\", \"produce help message\");\n  boost::program_options::store(\n    boost::program_options::parse_command_line(argc, argv, options_description),\n    options\n  );\n  boost::program_options::notify(options);\n  if (options.count(\"help\"))\n    cout << \"usage: \" << argv[0] << \" [options]\" << endl << options_description;\n  return 0;\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_scaffolds_task_main_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsTaskMainCpp(renderer, vars).render();
}