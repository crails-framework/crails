#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectConfigI18nCpp : public Crails::Template
{
public:
  ProjectConfigI18nCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include <crails/i18n.hpp>\n\nusing namespace std;\nusing namespace i18n;\n\nconst string         Settings::default_locale = \"en\";\nconst vector<string> Settings::locales = {\"en\", \"fr\", \"ru\"};\nthread_local string  Settings::current_locale;\nconst I18nData       Settings::t;\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_i18n_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectConfigI18nCpp(renderer, target, vars).render();
}