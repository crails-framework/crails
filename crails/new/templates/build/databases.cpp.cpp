#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigDatabasesCpp : public Crails::Template
{
public:
  render_ProjectConfigDatabasesCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include \"databases.hpp\"\n\nusing namespace Crails;\nusing namespace std;\n\nconst Databases::SettingsMap ApplicationDatabases::settings = {\n  {\n    Production, {\n    }\n  },\n\n  {\n    Development, {\n    }\n  },\n\n  {\n    Test, {\n    }\n  }\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_databases_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigDatabasesCpp(renderer, target, vars).render();
}