#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigDatabasesHpp : public Crails::Template
{
public:
  render_ProjectConfigDatabasesHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/databases.hpp>\n\nclass ApplicationDatabases : public Crails::Databases::Settings\n{\n  SINGLETON_IMPLEMENTATION(ApplicationDatabases, Crails::Databases::Settings)\n  static const Crails::Databases::SettingsMap settings;\npublic:\n  ApplicationDatabases() : Crails::Databases::Settings(settings)\n  {\n  }\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_databases_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigDatabasesHpp(renderer, target, vars).render();
}