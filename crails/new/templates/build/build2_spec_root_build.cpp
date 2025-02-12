#include <sstream>
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
ecpp_stream << "using cxx\n\nhxx{*}: extension = hpp\nixx{*}: extension = ipp\ntxx{*}: extension = tpp\ncxx{*}: extension = cpp\n\nexe{*}: test = true\n\ntest.target = $cxx.target\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_build2_spec_root_build(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_Build2SpecRootBuild(renderer, target, vars).render();
}