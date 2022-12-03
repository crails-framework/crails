#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectAppControllersApplicationHpp : public Crails::Template
{
public:
  ProjectAppControllersApplicationHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#ifndef  APPLICATION_CONTROLLER_HPP\n# define APPLICATION_CONTROLLER_HPP\n\n# include <crails/controller.hpp>\n\nclass ApplicationController : public Crails::Controller\n{\nprotected:\n  ApplicationController(Crails::Context& context) : Crails::Controller(context)\n  {\n    // Uncomment the following line to set `layouts/application` as your default layout\n    // Use `crails scaffold layout` to generate a layout using Bootstrap\n    //vars[\"layout\"] = std::string(\"layouts/application\");\n  }\n\n  // Uncomment the following line to disable CSRF protection (useful for webservices)\n  //virtual bool must_protect_from_forgery() const override { return false; }\n};\n\n#endif\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_app_controllers_application_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectAppControllersApplicationHpp(renderer, target, vars).render();
}