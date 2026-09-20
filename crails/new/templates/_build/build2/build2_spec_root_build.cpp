#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_Build2SpecRootBuild : public Crails::Template
{
public:
  render_Build2SpecRootBuild(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(366);
    // BEGIN TEMPLATE BODY
ecpp_stream << "using cxx\n\nhxx{*}: extension = hpp\nixx{*}: extension = ipp\ntxx{*}: extension = tpp\ncxx{*}: extension = cpp\n\nexe{*}: test = true\n\ntest.target = $cxx.target\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_build2_spec_root_build(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_Build2SpecRootBuild(renderer, target, vars).render();
}