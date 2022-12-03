#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class SidekixSidetasksCpp : public Crails::Template
{
public:
  SidekixSidetasksCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include \"sidetasks.hpp\"\n\nusing namespace std;\nusing namespace Crails;\n\n// Sidetask declarations\n\nconst map<string, function<void(Params&)>> sidetasks = {\n  // Sidetask map\n};\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_sidekix_sidetasks_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  SidekixSidetasksCpp(renderer, target, vars).render();
}