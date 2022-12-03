#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectAppRoutesCpp : public Crails::Template
{
public:
  ProjectAppRoutesCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include <crails/router.hpp>\n\nvoid Crails::Router::initialize(void)\n{\n  // Append routes here (do not remove this line)\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_app_routes_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectAppRoutesCpp(renderer, target, vars).render();
}