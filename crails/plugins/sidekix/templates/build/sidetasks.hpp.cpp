#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class SidekixSidetasksHpp : public Crails::Template
{
public:
  SidekixSidetasksHpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#pragma once\n#include <map>\n#include <crails/params.hpp>\n\nextern const std::map<std::string, std::function<void (Crails::Params&)> > sidetasks;\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_sidekix_sidetasks_hpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return SidekixSidetasksHpp(renderer, vars).render();
}