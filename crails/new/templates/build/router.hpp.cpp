#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigRouterHpp : public Crails::Template
{
public:
  render_ProjectConfigRouterHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/router.hpp>\n\nclass ApplicationRouter : public Crails::Router\n{\n  SINGLETON_IMPLEMENTATION(ApplicationRouter, Crails::Router)\npublic:\n  ApplicationRouter();\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_router_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigRouterHpp(renderer, target, vars).render();
}