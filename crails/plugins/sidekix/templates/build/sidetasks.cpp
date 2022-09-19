#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class SidekixSidetasksCpp : public Crails::Template
{
public:
  SidekixSidetasksCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#include \"sidetasks.hpp\"\n\nusing namespace std;\nusing namespace Crails;\n\n// Sidetask declarations\n\nconst map<string, function<void(Params&)>> sidetasks = {\n  // Sidetask map\n};\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_sidekix_sidetasks_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return SidekixSidetasksCpp(renderer, vars).render();
}