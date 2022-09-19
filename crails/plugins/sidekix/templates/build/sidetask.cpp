#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class SidekixSidetaskCpp : public Crails::Template
{
public:
  SidekixSidetaskCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    function_name(Crails::cast<std::string>(vars, "function_name"))
  {}

  std::string render()
  {
ecpp_stream << "#include <crails/params.hpp>\n\nvoid " << ( function_name );
  ecpp_stream << "(Crails::Params& params)\n{\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string function_name;
};

std::string render_sidekix_sidetask_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return SidekixSidetaskCpp(renderer, vars).render();
}