#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigI18nCpp : public Crails::Template
{
public:
  render_ProjectConfigI18nCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include <crails/i18n.hpp>\n#include <filesystem>\n\nusing namespace std;\nusing namespace i18n;\n\nApplicationI18n::ApplicationI18n()\n{\n  directory = filesystem::current_path().string();\n  default_locale = \"en\";\n  locales = {\"en\", \"es\", \"fr\"};\n  t.add_locale(\"en\", \"locales/en.json\");\n  t.add_locale(\"es\", \"locales/es.json\");\n  t.add_locale(\"fr\", \"locales/fr.json\");\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_i18n_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigI18nCpp(renderer, target, vars).render();
}