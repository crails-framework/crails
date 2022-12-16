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
    has_router( configuration_type == "webservice" || configuration_type == "full")
  {}

  void render()
  {
ecpp_stream << "#include <crails/tests/runner.hpp>";
 if (has_router){
  ecpp_stream << "\n#include <crails/router.hpp>";
 };
  ecpp_stream << "\n\nvoid Crails::Tests::Runner::setup()\n{";
 if (has_router){
  ecpp_stream << "\n  Router::singleton::initialize();\n  Router::singleton::get()->initialize();";
 };
  ecpp_stream << "\n}\n\nvoid Crails::Tests::Runner::shutdown()\n{";
 if (has_router){
  ecpp_stream << "\n  Router::singleton::finalize();";
 };
  ecpp_stream << "\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string configuration_type;
  bool has_router;
};

void render_project_spec_main_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectSpecMainCpp(renderer, target, vars).render();
}