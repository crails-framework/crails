#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectConfigDatabasesCpp : public Crails::Template
{
public:
  ProjectConfigDatabasesCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include <crails/databases.hpp>\n\nusing namespace Crails;\nusing namespace std;\n\nconst Databases::Settings Databases::settings = {\n  {\n    Production, {\n    }\n  },\n\n  {\n    Development, {\n    }\n  },\n\n  {\n    Test, {\n    }\n  }\n};\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_databases_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectConfigDatabasesCpp(renderer, target, vars).render();
}