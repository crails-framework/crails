#include <crails/template_stream.hpp>
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
    ecpp_stream.reserve(376);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include \"sidetasks.hpp\"\n\nusing namespace std;\nusing namespace Crails;\n\n// Sidetask declarations\n\nconst Sidekix::Sidetasks sidetasks = {\n  // Sidetask map\n};\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_sidekix_sidetasks_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_SidekixSidetasksCpp(renderer, target, vars).render();
}