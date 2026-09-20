#include <crails/template_stream.hpp>
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
    ecpp_stream.reserve(550);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include \"databases.hpp\"\n\nusing namespace Crails;\nusing namespace std;\n\nconst Databases::SettingsMap ApplicationDatabases::settings = {\n  {\n    Production, {\n    }\n  },\n\n  {\n    Development, {\n    }\n  },\n\n  {\n    Test, {\n    }\n  }\n};\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_project_config_databases_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigDatabasesCpp(renderer, target, vars).render();
}