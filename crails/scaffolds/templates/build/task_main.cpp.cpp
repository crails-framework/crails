#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldsTaskMainCpp : public Crails::Template
{
public:
  render_ScaffoldsTaskMainCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include <boost/program_options.hpp>\n#include <iostream>\n\nusing namespace std;\n\nint main(int argc, char** argv)\n{\n  boost::program_options::options_description options_description(\"Options\");\n  boost::program_options::variables_map options;\n\n  options_description.add_options()(\"help,h\", \"produce help message\");\n  boost::program_options::store(\n    boost::program_options::parse_command_line(argc, argv, options_description),\n    options\n  );\n  boost::program_options::notify(options);\n  if (options.count(\"help\"))\n    cout << \"usage: \" << argv[0] << \" [options]\" << endl << options_description;\n  return 0;\n}\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_scaffolds_task_main_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsTaskMainCpp(renderer, target, vars).render();
}