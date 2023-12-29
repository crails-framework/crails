#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_SidekixSidetasksCpp : public Crails::Template
{
public:
  render_SidekixSidetasksCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include \"sidetasks.hpp\"\n\nusing namespace std;\nusing namespace Crails;\n\n// Sidetask declarations\n\nconst map<string, function<void(Params&)>> sidetasks = {\n  // Sidetask map\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_sidekix_sidetasks_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_SidekixSidetasksCpp(renderer, target, vars).render();
}