#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectConfigI18nCpp : public Crails::Template
{
public:
  ProjectConfigI18nCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#include <crails/i18n.hpp>\n\nusing namespace std;\nusing namespace i18n;\n\nconst string         Settings::default_locale = \"en\";\nconst vector<string> Settings::locales = {\"en\", \"fr\", \"ru\"};\nthread_local string  Settings::current_locale;\nconst I18nData       Settings::t;\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_project_config_i18n_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectConfigI18nCpp(renderer, vars).render();
}