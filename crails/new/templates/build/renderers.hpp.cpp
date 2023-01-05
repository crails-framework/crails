#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigRenderersHpp : public Crails::Template
{
public:
  render_ProjectConfigRenderersHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/renderer.hpp>\n\nclass ApplicationRenderers : public Crails::Renderers\n{\n  SINGLETON_IMPLEMENTATION(ApplicationRenderers, Crails::Renderers)\npublic:\n  ApplicationRenderers();\n};\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_renderers_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigRenderersHpp(renderer, target, vars).render();
}