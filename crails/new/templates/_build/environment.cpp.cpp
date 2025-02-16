#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigEnvironmentCpp : public Crails::Template
{
public:
  render_ProjectConfigEnvironmentCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    environment(Crails::cast< std::string >(vars, "environment",  "Development"))
  {}

  void render()
  {
ecpp_stream << "#include <crails/environment.hpp>\n\nnamespace Crails\n{\n  Environment environment = " << ( environment );
  ecpp_stream << ";\n}\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string environment;
};

void render_project_config_environment_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigEnvironmentCpp(renderer, target, vars).render();
}