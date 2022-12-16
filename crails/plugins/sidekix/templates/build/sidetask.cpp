#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_SidekixSidetaskCpp : public Crails::Template
{
public:
  render_SidekixSidetaskCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    function_name(Crails::cast<std::string>(vars, "function_name"))
  {}

  void render()
  {
ecpp_stream << "#include <crails/params.hpp>\n\nvoid " << ( function_name );
  ecpp_stream << "(Crails::Params& params)\n{\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string function_name;
};

void render_sidekix_sidetask_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_SidekixSidetaskCpp(renderer, target, vars).render();
}