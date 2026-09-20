#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_SidekixSidetasksHpp : public Crails::Template
{
public:
  render_SidekixSidetasksHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(238);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#pragma once\n#include <crails/sidekix/sidetasks.hpp>\n\nextern const Sidekix::Sidetasks sidetasks;\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_sidekix_sidetasks_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_SidekixSidetasksHpp(renderer, target, vars).render();
}