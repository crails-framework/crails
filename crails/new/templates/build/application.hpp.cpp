#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectAppControllersApplicationHpp : public Crails::Template
{
public:
  ProjectAppControllersApplicationHpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#ifndef  APPLICATION_CONTROLLER_HPP\n# define APPLICATION_CONTROLLER_HPP\n\n# include <crails/controller.hpp>\n\nclass ApplicatonController : public Crails::Controller\n{\nprotected:\n  ApplicationController(Crails::Context& context) : Crails::Controller(context)\n  {\n  }\n\n  // Uncomment the following line to disable CSRF protection (useful for webservices)\n  //virtual bool must_protect_from_forgery() const override { return false; }\n};\n\n#endif\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_project_app_controllers_application_hpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectAppControllersApplicationHpp(renderer, vars).render();
}