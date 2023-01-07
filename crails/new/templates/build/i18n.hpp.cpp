#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigI18nHpp : public Crails::Template
{
public:
  render_ProjectConfigI18nHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/i18n.hpp>\n\nclass ApplicationI18n : public i18n::Settings\n{\n  SINGLETON_IMPLEMENTATION(ApplicationI18n, i18n::Settings)\n  ApplicationI18n();\n};\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_i18n_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigI18nHpp(renderer, target, vars).render();
}