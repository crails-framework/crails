#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectSpecMainCpp : public Crails::Template
{
public:
  render_ProjectSpecMainCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    configuration_type(Crails::cast<std::string>(vars, "configuration_type")), 
    with_action(Crails::cast<bool>(vars, "with_action",  true)), 
    with_databases(Crails::cast<bool>(vars, "with_databases",  false))
  {}

  void render()
  {
ecpp_stream << "#include <crails/tests/runner.hpp>\n#include \"config/server.hpp\"\n#include \"config/renderers.hpp\"";
 if (with_databases){
  ecpp_stream << "\n#include \"config/databases.hpp\"";
 };
  ecpp_stream << "";
 if (with_action){
  ecpp_stream << "\n#include \"config/router.hpp\"";
 };
  ecpp_stream << "\n\nvoid Crails::Tests::Runner::setup()\n{\n  ApplicationServer::singleton::initialize();\n  ApplicationRenderers::singleton::initialize();";
 if (with_databases){
  ecpp_stream << "\n  ApplicationDatabases::singleton::initialize();";
 };
  ecpp_stream << "";
 if (with_action){
  ecpp_stream << "\n  ApplicationRouter::singleton::initialize();";
 };
  ecpp_stream << "\n}\n\nvoid Crails::Tests::Runner::shutdown()\n{\n  ApplicationServer::singleton::finalize();\n  ApplicationRenderers::singleton::finalize();";
 if (with_databases){
  ecpp_stream << "\n  ApplicationDatabases::singleton::finalize();";
 };
  ecpp_stream << "";
 if (with_action){
  ecpp_stream << "\n  ApplicationRouter::singleton::finalize();";
 };
  ecpp_stream << "\n}\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string configuration_type;
  bool with_action;
  bool with_databases;
};

void render_project_spec_main_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectSpecMainCpp(renderer, target, vars).render();
}