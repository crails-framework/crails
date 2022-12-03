#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class SidekixSidetasksHpp : public Crails::Template
{
public:
  SidekixSidetasksHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <map>\n#include <crails/params.hpp>\n\nextern const std::map<std::string, std::function<void (Crails::Params&)> > sidetasks;\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_sidekix_sidetasks_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  SidekixSidetasksHpp(renderer, target, vars).render();
}