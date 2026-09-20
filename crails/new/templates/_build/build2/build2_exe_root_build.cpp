#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_Build2ExeRootBuild : public Crails::Template
{
public:
  render_Build2ExeRootBuild(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(262);
    // BEGIN TEMPLATE BODY
ecpp_stream << "using cxx\n\nhxx{*}: extension = hpp\nixx{*}: extension = ipp\ntxx{*}: extension = tpp\ncxx{*}: extension = cpp\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_build2_exe_root_build(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_Build2ExeRootBuild(renderer, target, vars).render();
}