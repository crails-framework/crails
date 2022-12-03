#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectConfigEnvironmentCpp : public Crails::Template
{
public:
  ProjectConfigEnvironmentCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    environment(Crails::cast<std::string>(vars, "environment",  "Development"))
  {}

  void render()
  {
ecpp_stream << "#include <crails/environment.hpp>\n\nnamespace Crails\n{\n  Environment environment = " << ( environment );
  ecpp_stream << ";\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string environment;
};

void render_project_config_environment_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectConfigEnvironmentCpp(renderer, target, vars).render();
}